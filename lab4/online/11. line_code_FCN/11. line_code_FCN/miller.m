%Program to simulate the Miller code.
%Author: Diego Barragán Guerrero
%www.matpic.com
%diegokillemall@yahoo.com
%h-->vector of bits
%est_initial-->initial state of the signal(may be 1 ó -1)
%-------------------------------------------------------------------
h=[1 1 0 0 1 0 1 1 0 0 0 1 1 1 1 0 1];%Señal
est_initial=-1;%Estado inicial
%-------------------------------------------------------------------
clc;%clear command window
close all;%Close all figures.
con=est_initial;%Set 1 ó -1
long=length(h);%Number of bits of the signal
n=1;%Initial state for "while" loop
ac=[];%Null matrix to code signal.
bits=[];%Null matrix to original signal.
h(long+1)=0;%Valor de extensión de la señal
while n<=long%Code to finished the length of the signal.
    if h(n)==1 %If the bit is 1
        bit=[ones(1,100)];
        s=[con*ones(1,50) -con*ones(1,50)];
        con=con*-1;%Switch state of the signal
    else %If the bit is 0
        bit=[zeros(1,100)];
        s=[con*ones(1,100)];
        if h(n+1)==0%If the next bit is 0
            con=con*-1;%Switch state of the signal
        end
    end
    ac=[ac s];%Accumulate miller code.
    bits=[bits bit];%Accumulate signal
    n=n+1;%Increment of the cycle
    s=[];%Reset temporal matrix s.
end
subplot(2,1,1);plot(bits,'LineWidth',2);
title('INPUT SIGNAL');
set(gca,'xtick',0:100:100*long)%
axis([0 100*(length(h)-1) -2 2])%
grid on %
%*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
subplot(2,1,2);plot(ac,'LineWidth',2)%
title('MILLER CODE')
set(gca,'xtick',0:100:100*long)%
axis([0 100*(length(h)-1) -2 2])%
grid on %
