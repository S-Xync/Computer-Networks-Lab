set ns [new Simulator]

$ns rtproto DV

set tracefile [open out.tr w]
$ns trace-all $tracefile

set namfile [open out.nam w]
$ns namtrace-all $namfile

proc finish {} {
	global 
}