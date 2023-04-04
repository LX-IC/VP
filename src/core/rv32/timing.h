#ifndef RISCV_VP_TIMING_H
#define RISCV_VP_TIMING_H

#include <stdint.h>

#include <vector>
#include <list>
#include <deque>

/*
 * Timing model for the HiFive1 pipeline.
 * This model only tracks timing related information (for example it is not necessary to keep track of different pipeline stages)
 * which essentially are:
 *
 * 1. registers that store results of operations that do not complete within a single cycle (i.e. essentially result dependencies between different instructions which may cause a pipeline stall).
 *    Operations that do not complete within a single execution cycle are: mult, div, load, store and csr access.
 *    Accessing such a register will stall the pipeline model, thus essentially advance the cycle counter to complete the pending operation.
 * 2. mult and div operations, since the HiFive1 has only a single multiplication and division unit.
 * 3. load and store addresses, since a load whose address overlaps with a still pending store requires longer time to finish on a HiFive1.
 */
struct PipelineTiming
{
    
    struct Range {
        unsigned start;
        unsigned end;

        bool contains(unsigned addr) const {
            assert (end >= start);
            return addr >= start && addr < end;
        }

        bool overlaps(const Range &other) const {
            if (other.start < start) {
                return other.end > start;
            } else {
                return end > other.start;
            }
        }

        unsigned size() const {
            return end-start;
        }
    };

    struct StoreReservation {
        Range range;
        int latency;

        bool contains(unsigned addr) const {
            return range.contains(addr);
        }

        bool overlaps(const Range &other) const {
            return range.overlaps(other);
        }

        bool advance(int num_cycles) {
            assert (latency > 0);
            latency -= num_cycles;
            if (latency <= 0) {
                return false;
            }
            return true;
        }
    };

    struct OperationReservation {
        bool acquired = false;
        int latency = 0;

        void acquire(int num_cycles) {
            assert (num_cycles > 0);
            assert (!acquired);
            acquired = true;
            latency = num_cycles;
        }

        void release() {
            assert (latency <= 0);
            assert (acquired);
            acquired = false;
        }

        bool advance(int num_cycles) {
            if (!acquired)
                return true;
            assert (latency > 0);
            latency -= num_cycles;
            if (latency <= 0) {
                release();
                return false;
            }
            return true;
        }
    };

    struct RegisterLatency {
        unsigned reg_idx;
        int latency;

        bool advance(int num_cycles) {
            assert (latency > 0);
            latency -= num_cycles;
            if (latency <= 0) {
                return false;
            }
            return true;
        }
    };

    std::vector<RegisterLatency> pending_register_latencies;
    OperationReservation mult;
    OperationReservation div;
    std::vector<StoreReservation> pending_store_reservations;
    uint64_t num_total_cycles;

    /*
     * Called by the ISS after execution of every instruction to advance the timing model
     * for one clock cycle.
     */
    void advance(int num_cycles=1) {
        assert (num_cycles > 0);

        {
            auto it = pending_register_latencies.begin();
            while (it != pending_register_latencies.end()) {
                if (!it->advance(num_cycles))
                    it = pending_register_latencies.erase(it);
                else
                    ++it;
            }
        }

        mult.advance(num_cycles);
        div.advance(num_cycles);

        {
            auto it = pending_store_reservations.begin();
            while (it != pending_store_reservations.end()) {
                if (!it->advance(num_cycles))
                    it = pending_store_reservations.erase(it);
                else
                    ++it;
            }
        }

        num_total_cycles += num_cycles;
    }

    void stall(int num_cycles) {
        advance(num_cycles);
    }

    /* Called by the ISS whenever a register is read or written.
     * Stalls the pipeline accordingly in case the register is used in a still pending operation. */
    void access_register(unsigned reg_idx) {
        for (const auto &e : pending_register_latencies) {
            if (e.reg_idx == reg_idx) {
                stall(e.latency);
                return;
            }
        }
    }

    bool _find(unsigned reg_idx) {
        unsigned idx = 0;
        for (const auto &e : pending_register_latencies) {
            if (e.reg_idx == reg_idx)
                return true;
            ++idx;
        }
        return false;
    }

    void add_result_latency(unsigned reg_idx, int result_latency) {
        assert (!_find(reg_idx));
        assert (result_latency > 0);
        pending_register_latencies.push_back({reg_idx, result_latency});
    }

    void reserve_mult(int num_cycles) {
        // HiFive1 has a single multiplication unit
        if (mult.acquired)
            stall(mult.latency);    // thus wait for previous mult to finish before performing the next mult
        mult.acquire(num_cycles);
    }

    int get_mult_result_latency(uint32_t a, uint32_t b) {
        // operation: a * b
        return 5+1;
    }

    // called by ISS on every multiplication
    void do_mult(unsigned result_reg_idx, int32_t a, int32_t b) {
        int num_cycles = get_mult_result_latency(a, b);
        reserve_mult(num_cycles);
        add_result_latency(result_reg_idx, num_cycles+1);
    }

    void reserve_div(int num_cycles) {
        // HiFive1 has a single division unit, similar to multiplication
        if (div.acquired)
            stall(div.latency);
        div.acquire(num_cycles);
    }

    int get_div_result_latency(uint32_t a, uint32_t b) {
        // operation: a / b
        // TODO: approximates the delay of a division, add a more precise model if necessary
        return (rand() % 5) + 6;
    }

    // called by ISS on every division
    void do_div(unsigned result_reg_idx, int32_t a, int32_t b) {
        int num_cycles = get_div_result_latency(a, b);
        reserve_div(num_cycles);
        add_result_latency(result_reg_idx, num_cycles);
    }

    // called by ISS on every CSR access
    void do_csr(unsigned result_reg_idx) {
        add_result_latency(result_reg_idx, 5);
    }

    // called by ISS on store instructions
    void reserve_store(Range addr_range, int num_cycles) {
        pending_store_reservations.push_back({addr_range, num_cycles+1});
    }

    bool has_store_reservation(const Range &addrs) {
        for (const auto &e : pending_store_reservations) {
            if (e.overlaps(addrs))
                return true;
        }
        return false;
    }

    // called by ISS on load instructions
    /*
     * A load instructions whose address overlaps with a still pending store instruction results in extra
     * execution penalty.
     */
    void do_load(const Range &addr_range, unsigned rd, int latency) {
        int extra = 0;
        if (has_store_reservation(addr_range)) {
            stall(2);       // load address overlaps previous still pending store address
            // if (addr_range.size() < 4)
            //     extra += 2; // non word size access takes longer, i.e. LB and LH
        }
        add_result_latency(rd, latency+extra);
    }

};

#endif