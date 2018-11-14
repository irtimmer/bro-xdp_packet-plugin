# @TEST-EXEC: bro -NN irtimmer::AF_XDP |sed -e 's/version.*)/version)/g' >output
# @TEST-EXEC: btest-diff output
