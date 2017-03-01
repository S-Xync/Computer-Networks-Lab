%code for manchestor encoding
input=[0 1 0 0 1 0 1 0];
output=[];
for i=input
  if i==0
    output=[output,1,0];
  elseif i==1
    output=[output,0,1];
  end
end
goutput=[];
gtime=[];
index=0;
c=1;
for t=0.01:0.01:length(input)
  if mod(c,50)==1
    index=index+1;
  end
  goutput(c)=output(index);
  gtime(c)=t;
  c=c+1;
end
plot(gtime,goutput,'marker','.','color','r');
title('Manchestor Encoding');
xlabel('time');
ylabel('volts');
