foreach mysite {www.wellho.net wellho.net www.wellho.co.uk www.firstgreatwestern.info} {
        spawn ping -c 3 $mysite
        expect "round-trip"
        regexp {time=([0-9]+\.[0-9]+)} $expect_out(buffer) all trip
        puts "$mysite came back with a trip time of $trip"
}

exit