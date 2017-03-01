input=[0 1 0 0 1 0 1 0];
output=[];
tran=0;
index=1;
for i=input
  if i==1
    tran=mod((tran+1),2);
  end
  output(index)=tran;
  index=index+1;
end
goutput=[];
gtime=[];
index=0;
c=1;
for t=0.01:0.01:length(input)
  if mod(c,100)==1
    index=index+1;
  end
  goutput(c)=output(index);
  gtime(c)=t;
  c=c+1;
end
plot(gtime,goutput,'marker','.','color','r');
title('NRZI Encoding');
xlabel('time');
ylabel('volts');