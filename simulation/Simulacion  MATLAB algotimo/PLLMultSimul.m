clc,
close all;
%PLL multiplicador algoritmo
fs=1e3;
Ts=1/fs;
win=2*pi*50;
phi_in=2*pi/8;
T=2*pi/(win) %un ciclo
N=20;         % Nro de ciclos de las señales

%Señal de entrada
t1=0:Ts:N*T;
figure(1)
vref=1*sin(win*t1+phi_in);
subplot(2,1,1)
plot(t1,vref)
%Fase de entrada
%figure(2)
subplot(2,1,2)
Fasein=win*t1;
plot(t1, Fasein)
%%
clc

disp('********* Inicio de varible ***************')
fase0=0
wL=2*pi*50
kc=T/Ts  %1 ciclo en discreto
disp('definición el punto de operación')
Pma=0  %

disp('********* Historias ***************')
disp(' instante k-1           instante k ')
disp('***********************************')
Dw_k_1=0      %sal del compensador en t=k-1
P_k_1=0       %producto, entrada al filtro en t=k-1
Pm_k_1=0      %sal del filtro en t=k-1
Em_k_1=0      %entrada al compensador en t=k-1

for k= 1:1:N*kc
   disp('***************************************************************')
   disp('            muestra de la entrada en el instante k             ')
        k
   disp('***************************************************************')
      % pause
       t=t1(1,k+1)
       muestra_in=vref(1,k+1)
       phin=win*t
   
   disp('*********  genero la señal vo ***************')
       w0= Dw_k_1+wL
       fase0= fase0+(w0*Ts)       %integro posible desborde
       v0=sin(fase0)               %genero la señal en el intante k
   
   disp('*********  Salida del multiplicador p ***************')
        P_k=muestra_in*v0 %multiplico las señales, sal del mult en k
   
    disp('*********  salida del filtro ***************')
    disp('ec en diferencia del filtro ')
    disp('Pm_k= 0.0618551*P_k_1+ 0.9690724*Pm_k_1')
        a1=0.0618551;
        a2=0.9690724;
        Pm_k=a1*P_k_1+a2*Pm_k_1
    disp('**********Error medio= Pm*-Pm******** ')
        Em_k=Pma- Pm_k_1  %en t=k %Entrada al compensador Error
    
    disp('*********  salida del compensador ***************')
    disp('%ec en diferencia del compensador en t=k')
    disp('Dw_k= 63.6619772*Em_k - 63.4619772*Em_k_1 + Dw_k_1')
        b1=63.6619772;
        b2=63.4619772;
        Dw_k= b1*Em_k - b2*Em_k_1 + Dw_k_1
    
    disp('*********  actualizo las historias ***************')
        P_k_1=P_k
        Pm_k_1=Pm_k
        Em_k_1=Em_k
        Dw_k_1 = Dw_k
    
        figure(1)
        subplot(2,1,1)
        hold on
        plot(k*Ts, v0,'*r')
        subplot(2,1,2)
        hold on
        plot(k*Ts, fase0,'*r')
    
end