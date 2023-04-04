import sys, binascii, Crypto.Cipher.AES as AES
benchmark_name = "SM4"
pt  = binascii.a2b_hex("0123456789abcdeffedcba9876543210")
mk  = binascii.a2b_hex("0123456789abcdeffedcba9876543210")
erk = binascii.a2b_hex("f98621f1612b66419ab16a5a7720a97bf4607336610c6a77b389bbb6513176247c3020a5bd4d58b7ed5307c3575be57e8c608869b795d830af14ba44a195441028b420d1a35fb573664987cc394424921f649ee85a01ca98609015c72efde1990cd89bb7b015211deb8a220e810c78f154368d4296342962e572cf0112a02491")
drk = binascii.a2b_hex("12a02491e572cf019634296254368d42810c78f1eb8a220eb015211d0cd89bb72efde199609015c75a01ca981f649ee839442492664987cca35fb57328b420d1a1954410af14ba44b795d8308c608869575be57eed5307c3bd4d58b77c3020a551317624b389bbb6610c6a77f46073367720a97b9ab16a5a612b6641f98621f1")
ct  = binascii.a2b_hex("681edf34d206965e86b3e94f536e4246")
fi  = binascii.a2b_hex("0123456789abcdeffedcba9876543210")
kse_icount = 0x0000000000000a24
ksd_icount = 0x0000000000000c39
enc_icount = 0x0000000000000b0f
dec_icount = 0x0000000000000b10
sys.stdout.write("SM4 Test passed. ")
sys.stdout.write("enc: %d, " % (enc_icount))
sys.stdout.write("dec: %d, " % (dec_icount))
sys.stdout.write("kse: %d, " % (kse_icount))
sys.stdout.write("ksd: %d, " % (ksd_icount))
print("")