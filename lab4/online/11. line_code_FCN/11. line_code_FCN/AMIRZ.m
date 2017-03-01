function AMIRZ(h)
%Line code AMI RZ.
%Author: Diego Orlando Barragán Guerrero
%For more information, visit: www.matpic.com
%diegokillemall@yahoo.com
%Example:
%h=[1 0 0 1 1 0 1 0 1 0];
%AMIRZ(h)

clf;
n=1;
l=length(h);
h(l+1)=1;
ami=-1;
while n<=length(h)-1;
    t=n-1:0.001:n;
if h(n) == 0
    if h(n+1)==0  
        y=(t>n);
    else
        if ami==1
            y=-(t==n);
        else
            y=(t==n);
        end
    end
    d=plot(t,y);grid on;
     title('Line code AMI RZ');
    set(d,'LineWidth',2.5);
    hold on;
    axis([0 length(h)-1 -1.5 1.5]);
    disp('zero');
else
    ami=ami*-1;
    if h(n+1)==0
        if ami==1
            y=(t<n-0.5);
        else
            y=-(t<n-0.5);
        end
    else
        if ami==1
            y=(t<n-0.5)-(t==n);
        else
            y=-(t<n-0.5)+(t==n);
        end
        
    end
    %y=(t>n-1)+(t==n-1);
    d=plot(t,y);grid on;
    title('Line code AMI RZ');
    set(d,'LineWidth',2.5);
    hold on;
    axis([0 length(h)-1 -1.5 1.5]);
    disp('one'); 
end
n=n+1;
%pause; 
end