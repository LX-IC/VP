############################SM4#####################################
test-sm4 : test_block_sm4.c sm4_reference.c test.c bootstrap.S
	riscv64-unknown-elf-gcc test_block_sm4.c sm4_reference.c test.c bootstrap.S -o test_sm4 -march=rv32i -mabi=ilp32 -nostartfiles -Wl,--no-relax
	
sim-sm4: test_sm4
	tiny32-vp --intercept-syscalls test_sm4

sm4-baseline : test-sm4 sim-sm4

test-sm4-peripheral : test_block_sm4_peri.c test.c bootstrap.S
	riscv64-unknown-elf-gcc test_block_sm4_peri.c test.c bootstrap.S -o test_sm4 -march=rv32i -mabi=ilp32 -nostartfiles -Wl,--no-relax

sm4-peripheral : test-sm4-peripheral sim-sm4	

############################SM3#####################################
test-sm3 : test_hash_sm3.c sm3.c test.c bootstrap.S
	riscv64-unknown-elf-gcc test_hash_sm3.c sm3.c test.c bootstrap.S -o test_sm3 -march=rv32i -mabi=ilp32 -nostartfiles -Wl,--no-relax

sim-sm3 : test_sm3
	tiny32-vp --intercept-syscalls test_sm3 > output.txt

check-test : output.txt output_process.py
	python3 output_process.py
	python3 check.py

sm3-baseline : test-sm3 sim-sm3 check-test

test-sm3-peripheral : test_hash_sm3_peri.c test.c bootstrap.S
	riscv64-unknown-elf-gcc test_hash_sm3_peri.c test.c bootstrap.S -o test_sm3 -march=rv32i -mabi=ilp32 -nostartfiles -Wl,--no-relax

sm3-peripheral : test-sm3-peripheral sim-sm3 check-test

############################AES#####################################
test-aes-128 : test_block_aes_128.c aes_dec.c aes_enc.c test.c bootstrap.S
	riscv64-unknown-elf-gcc test_block_aes_128.c aes_dec.c aes_enc.c test.c bootstrap.S -o test_aes_128 -march=rv32i -mabi=ilp32 -nostartfiles -Wl,--no-relax

sim-aes-128 : test_aes_128
	tiny32-vp --intercept-syscalls test_aes_128 > output.txt

aes-128-baseline : test-aes-128 sim-aes-128 check-test

test-aes-192 : test_block_aes_192.c aes_dec.c aes_enc.c test.c bootstrap.S
	riscv64-unknown-elf-gcc test_block_aes_192.c aes_dec.c aes_enc.c test.c bootstrap.S -o test_aes_192 -march=rv32i -mabi=ilp32 -nostartfiles -Wl,--no-relax

sim-aes-192 : test_aes_192
	tiny32-vp --intercept-syscalls test_aes_192 > output.txt

aes-192-baseline : test-aes-192 sim-aes-192 check-test

test-aes-256 : test_block_aes_256.c aes_dec.c aes_enc.c test.c bootstrap.S
	riscv64-unknown-elf-gcc test_block_aes_256.c aes_dec.c aes_enc.c test.c bootstrap.S -o test_aes_256 -march=rv32i -mabi=ilp32 -nostartfiles -Wl,--no-relax

sim-aes-256 : test_aes_256
	tiny32-vp --intercept-syscalls test_aes_256 > output.txt

aes-256-baseline : test-aes-256 sim-aes-256 check-test

############################SHA3#####################################
test-sha3 : test_hash_sha3.c Keccak.c fips202.c test.c bootstrap.S
	riscv64-unknown-elf-gcc test_hash_sha3.c Keccak.c fips202.c test.c bootstrap.S -o test_sha3 -march=rv32i -mabi=ilp32 -nostartfiles -Wl,--no-relax

sim-sha3 : test_sha3
	tiny32-vp --intercept-syscalls test_sha3 > output.txt

sha3-baseline : test-sha3 sim-sha3 check-test


############################SHA256##################################
test-sha256 : test_hash_sha256.c sha256.c test.c bootstrap.S
	riscv64-unknown-elf-gcc test_hash_sha256.c sha256.c test.c bootstrap.S -o test_sha256 -march=rv32i -mabi=ilp32 -nostartfiles -Wl,--no-relax

sim-sha256 : test_sha256
	tiny32-vp --intercept-syscalls test_sha256 > output.txt

sha256-baseline : test-sha256 sim-sha256 check-test

############################SHA512##################################
test-sha512 : test_hash_sha512.c sha512.c test.c bootstrap.S
	riscv64-unknown-elf-gcc test_hash_sha512.c sha512.c test.c bootstrap.S -o test_sha512 -march=rv32i -mabi=ilp32 -nostartfiles -Wl,--no-relax

sim-sha512 : test_sha512
	tiny32-vp --intercept-syscalls test_sha512 > output.txt

sha512-baseline : test-sha512 sim-sha512 check-test


####################################################################
dump-elf: all
	riscv32-unknown-elf-readelf -a main
	
dump-code: all
	riscv32-unknown-elf-objdump -D main
	
dump-comment: all
	objdump -s --section .comment main
	
clean:
	rm -f main
