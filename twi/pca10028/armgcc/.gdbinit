target remote localhost:2331
mon speed 10000
mon endian little
mon flash download=1
mon flash device=NRF51422
mon reset 0
set confirm off
set output-radix 16
file _build/nrf51422_xxac.out
