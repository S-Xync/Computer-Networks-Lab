function MANCHESTER(h)
%Line code MANCHESTER
%Author: Diego Orlando Barrag�n Guerrero
%For more information, visit: www.matpic.com
%diegokillemall@yahoo.com
%Example:
%h=[1 0 0 1 1 0 1 0 1 0];
%MANCHESTER(h)
clf;
n=1;
h=~h;
l=length(h);
h(l+1)=1;
while n<=length(h)-1;
    t=n-1:0.001:n;
if h(n) == 0
    if h(n+1)==0  
        y=-(t<n)+2*(t<n-0.5)+1*(t==n);
    else
        y=-(t<n)+2*(t<n-0.5)-1*(t==n);
    end
    d=plot(t,y);grid on;
    title('Line code MANCHESTER');
    set(d,'LineWidth',2.5);
    hold on;
    axis([0 length(h)-1 -1.5 1.5]);
    disp('one');
else
    if h(n+1)==0
        y=(t<n)-2*(t<n-0.5)+1*(t==n);
    else
        y=(t<n)-2*(t<n-0.5)-1*(t==n);
    end
    %y=(t>n-1)+(t==n-1);
    d=plot(t,y);grid on;
    title('Line code MANCHESTER');
    set(d,'LineWidth',2.5);
    hold on;
    axis([0 length(h)-1 -1.5 1.5]);
    disp('zero'); 
end
n=n+1;
%pause; 
end

