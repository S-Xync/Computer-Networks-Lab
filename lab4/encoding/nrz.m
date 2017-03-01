input=[0 1 0 0 1 0 1 0];
goutput=[];
gtime=[];
index=0;
c=1;
for t=0.01:0.01:length(input)
  if mod(c,100)==1
    index=index+1;
  end
  goutput(c)=input(index);
  gtime(c)=t;
  c=c+1;
end
plot(gtime,goutput,'marker','.','color','r');
title('NRZ Encoding');
xlabel('time');
ylabel('volts');