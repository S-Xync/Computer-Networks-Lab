function URZ(h)
%Line code UNIPOLAR RZ.
%Author: Diego Orlando Barragán Guerrero
%For more information, visit: www.matpic.com
%diegokillemall@yahoo.com
%Example:
%h=[1 0 0 1 1 0 1 0 1 0];
%URZ(h)
clf;
n=1;
l=length(h);
h(l+1)=1;
while n<=length(h)-1;
    t=n-1:0.001:n;
%Graficación de los CEROS (0)
    if h(n) == 0
        if h(n+1)==0  
            y=(t>n);
        else
            y=(t==n);
        end
    d=plot(t,y);grid on
    title('Line code UNIPOLAR RZ');
    set(d,'LineWidth',2.5);
    hold on;
    axis([0 length(h)-1 -1.5 1.5]);
    disp('zero');
%Graficación de los UNOS (1)
    else
    if h(n+1)==0
        %y=(t>n-1)-2*(t==n);
        y=(t<n-0.5);
    else
        %y=(t>n-1)+(t==n-1);
        y=(t<n-0.5)+1*(t==n);
    end
    %y=(t>n-1)+(t==n-1);
    d=plot(t,y);grid on;
    title('Line code UNIPOLAR RZ');
    set(d,'LineWidth',2.5);
    hold on;
    axis([0 length(h)-1 -1.5 1.5]);
    disp('one'); 
end
n=n+1;
%pause; 
end
end