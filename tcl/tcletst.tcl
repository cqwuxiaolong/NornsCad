set curtime [clock seconds]
puts $curtime
ivtkinit
for {set i 0} {$i < 100000} {incr i} {
  box b 10 10 10
}
set curtime [clock seconds]
puts $curtime