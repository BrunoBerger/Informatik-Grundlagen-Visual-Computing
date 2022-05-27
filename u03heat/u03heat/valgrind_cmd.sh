valgrind --tool=callgrind --demangle=yes \
    --read-inline-info=yes --read-var-info=yes \
    --callgrind-out-file=heat.callgrind.out \
    --dump-line=yes --dump-instr=yes \
    --compress-strings=no --compress-pos=no \
    --collect-jumps=yes --cache-sim=yes \
    --branch-sim=yes \
    ./heat
