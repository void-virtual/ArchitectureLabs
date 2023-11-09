#/bin/sh
set +euo pipefail

for i in 1 2 5 10; do 
    printf "================================ Using ${i} connections and threads\n"
    printf 'No cache\t'
    wrk -d 10 -t ${i} -c ${i} --latency -s ./get_no_cache.lua http://localhost:8080/ | grep 'requests in'
    printf 'With cache\t'
    wrk -d 10 -t ${i} -c ${i} --latency -s ./get_cache.lua http://localhost:8080/ | grep 'requests in'
    printf '\n'
done
