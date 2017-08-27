set Version "1.21"
 set Date "29-Aug-2006"
 

 
 proc TheTime {} \
 {
   return [clock format [clock seconds] -format "%I:%M:%S %p on %a %d-%h-%Y"]
 }
 
 proc TheDate {} \
 {
   return [clock format [clock seconds] -format "%Y-%h-%d"]
 }
 
 proc Ping {name} \
 {
   puts "Ping test: $name"
   puts "->Performing test..."
   set Data [exec ping -n 1 $name]
   puts "->Parsing results..."
 
   set Lines [split $Data "\n"]
   set Data [lindex $Lines 6] ; puts "($Data)"
 
   if {$Data=="Request timed out."} {return -1}
 
   set Fields [split $Data ":"]
   set Fields [split [lindex $Fields 1] " "]
 
   set Time [string range [lindex $Fields 2] 5 end]
   set TTL [string range [lindex $Fields 3] 4 end]
 
   set Time [string trimright $Time "ms"]
 
   puts "->Ping time was '$Time' milliseconds."
   puts "->TTL of reply was '$TTL' hops."
 
   return $Time
 }
 
 proc GetIP {dns} \
 {
   puts "Get IP address: '$dns'"
   puts "->Performing ping test..."
   set Data [exec ping -n 1 $dns]
   puts "->Parsing results..."
 
   set Lines [split $Data "\n"]
   set Data [lindex $Lines 6] ; puts "($Data)"
 
   if {$Data=="Request timed out."} {return ""}
 
   set Fields [split $Data ":"]
   set Data [lindex $Fields 0]
   set Fields [split $Data " "]
 
   return [lindex $Fields 2]
 }
 
 proc AddLine {name label value} \
 {
   frame $name
   label "$name.l" -text $label -font "Times 12 bold"  ; pack "$name.l" -side left
   label "$name.v" -text $value -font "Times 12 roman" ; pack "$name.v" -side right
   pack $name -side top -expand yes -fill x
 }
 
 proc AddSlider {name label min max res} \
 {
   frame $name
   label "$name.l" -text $label -font "Times 12 bold"     ; pack "$name.l" -side left
   scale "$name.v" -orient horizontal -from $min -to $max -resolution $res -length 300 ; pack "$name.v" -side right
   pack $name -side top -expand yes -fill x
 }
 
 set RUN 0
 
 proc Do {name} \
 {
   global RUN Version
   set RUN 1
 
   set IP [GetIP $name]
   if {$IP==""} {puts "DNS lookup error?" ; return ""}
   wm title . "Pinger $Version: $name ($IP)"
 
   AddLine .dst "Destination:" "$name ($IP)"
   AddLine .start "Started at:" [clock format [clock seconds] -format "%I:%M:%S %p (%Z) on %a %d-%h-%Y"]
   AddLine .count1 "Total ping packets sent:" 0
   AddLine .count2 "Total ping packets received:" 0
   AddLine .count3 "Total ping packets lost:" 0
   AddSlider .min_t "Minimum ping time (ms):" 0 500 1
   AddSlider .mid_t "Arith. mean ping time (ms):" 0 500 0.1
   AddSlider .max_t "Maximum ping time (ms):" 0 500 1
   AddSlider .now_t "Current ping time (ms):" 0 500 1
   AddSlider .sd_t  "Ping time standard deviation:" 0 200 0.01
   AddLine .prev_good_h  "Most recent run of successful pings..." ""
   AddLine .prev_good_t1 "\u2192Started:" "(No successes)"
   AddLine .prev_good_t2 "\u2192Ended:" "(No successes)"
   AddLine .prev_good_no "\u2192Total packets:" "-"
   AddLine .prev_bad_h  "Most recent run of failed pings..." ""
   AddLine .prev_bad_t1 "\u2192Started:" "(No failures)"
   AddLine .prev_bad_t2 "\u2192Ended:" "(No failures)"
   AddLine .prev_bad_no "\u2192Total packets:" "-"
   AddLine .mode "Last ping result:" "Untested"
 
   button .stop -text "Stop" -command {set RUN 0 ; puts "STOP!"}
   pack .stop -side top
 
   button .debug -text "DEBUG" -command {console show}
   pack .debug -side bottom
 
   set Count1 0
   set Count2 0
   set Count3 0
   set MinT 0
   set MaxT 0
   set ListT [list]
   set PrevGoodC 0
   set PrevBadC 0
   set Mode ""
   set PAUSE 0
 
   set LOG [open "Log--[TheDate]--$name.txt" w]
   puts $LOG "[TheTime] Pinger $Version: Now pinging '$name' ($IP)"
 
   while {$RUN==1} \
   {
     incr Count1 ; .count1.v configure -text $Count1
     .stop configure -bg #FF0000
     update
 
     set T [Ping $IP]
     .stop configure -bg #00FF00
 
     if {$T > 0} \
     {
       incr Count2 ; .count2.v configure -text $Count2
 
       .now_t.v set $T
 
       if {$MinT == 0} {set MinT $T ; .min_t.v set $MinT}
 
       if {$T < $MinT} {set MinT $T ; .min_t.v set $MinT}
       if {$T > $MaxT} {set MaxT $T ; .max_t.v set $MaxT}
 
       set ListT [lrange $ListT 0 255] ; lappend ListT $T
       set MidT 0
       set RMST 0
       foreach Time $ListT {incr MidT $Time ; incr RMST [expr $Time * $Time]}
       set MidT [expr double($MidT) / double([llength $ListT])] ; .mid_t.v set $MidT
       set RMST [expr double($RMST) / double([llength $ListT])]
       set SD [expr sqrt($RMST - $MidT*$MidT)] ; .sd_t.v set $SD
 
       .prev_good_t2.v configure -text [TheTime]
       incr PrevGoodC ; .prev_good_no.v configure -text $PrevGoodC
 
       if {$PrevGoodC % 200 == 0} \
       {
         LogGood $LOG $PrevGoodC $MinT $MaxT $MidT $SD yes
         puts $LOG [TheTime]
         flush $LOG
       }
 
       if {$Mode!="Good"} \
       {
         set Mode "Good" ; .mode.v configure -text "Success" -fg #00FF00
 
         .prev_good_t1.v configure -text [TheTime]
 
         puts $LOG "  $PrevBadC packet(s) failed."
 
         set PrevBadC 0
 
         puts $LOG "[TheTime] Successful ping!"
         flush $LOG
       }
     } \
     else \
     {
       incr Count3 ; .count3.v configure -text $Count3
 
       .prev_bad_t2.v configure -text [TheTime]
       incr PrevBadC ; .prev_bad_no.v configure -text $PrevBadC
 
       if {$Mode!="Bad"} \
       {
         set Mode "Bad" ; .mode.v configure -text "Failure" -fg #FF0000
 
         .prev_bad_t1.v configure -text [TheTime]
 
         LogGood $LOG $PrevGoodC $MinT $MaxT $MidT $SD no
 
         set PrevGoodC 0
         set MinT 0
         set MaxT 0
         set ListT [list]
 
         puts $LOG "[TheTime] Failed ping!"
         flush $LOG
       }
     }
 
     after 2000 {set PAUSE 0}
     vwait PAUSE
   }
 
   if {$Mode=="Bad"}  {puts $LOG "  $PrevBadC packet(s) failed."}
   if {$Mode=="Good"} {LogGood $LOG $PrevGoodC $MinT $MaxT $MidT $SD no}
 
   puts $LOG "[TheTime] User abort."
   puts $LOG "  $Count1 ping packet(s) sent."
   puts $LOG "  $Count2 reply packet(s) received."
   puts $LOG "  $Count3 reply packet(s) NOT received."
   close $LOG
   exit
 }
 
 proc LogGood {Handle Count Min Max Av SD Cont} \
 {
   if {$Cont==yes} \
   {puts $Handle "  $Count packets successful and counting..."} \
   {puts $Handle "  $Count packet(s) successful."}
 
   puts $Handle "  $Min ms round trip (minimum)."
   puts $Handle "  $Av ms round trip (arithmatic mean)."
   puts $Handle "  $Max ms round trip (maximum)."
   puts $Handle "  $SD ms standard deviation."
 }
 
 puts "Do <ip>"
 puts "Do <dns name>"