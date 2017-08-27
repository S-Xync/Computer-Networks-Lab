BEGIN {
   2:        recvdSize = 0
   3:        startTime = 400
   4:        stopTime = 0
   5:   }
   6:    
   7:   {
   8:              event = $1
   9:              time = $2
  10:              node_id = $3
  11:              pkt_size = $8
  12:              level = $4
  13:    
  14:   # Store start time
  15:   if (level == "AGT" &;& event == "s" && pkt_size >= 512) {
  16:     if (time <; startTime) {
  17:              startTime = time
  18:              }
  19:        }
  20:    
  21:   # Update total received packets' size and store packets arrival time
  22:   if (level == "AGT" &;& event == "r" && pkt_size >= 512) {
  23:        if (time >; stopTime) {
  24:              stopTime = time
  25:              }
  26:        # Rip off the header
  27:        hdr_size = pkt_size % 512
  28:        pkt_size -= hdr_size
  29:        # Store received packet's size
  30:        recvdSize += pkt_size
  31:        }
  32:   }
  33:    
  34:   END {
  35:        printf("Average Throughput[kbps] = %.2f\t\t StartTime=%.2f\tStopTime=%.2f\n",(recvdSize/(stopTime-startTime))*(8/1000),startTime,stopTime)
  36:   }
