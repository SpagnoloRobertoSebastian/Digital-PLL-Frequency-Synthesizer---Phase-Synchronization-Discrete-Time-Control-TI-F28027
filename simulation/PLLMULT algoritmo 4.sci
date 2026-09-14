clc, clear, xdel(winsid());

//PLL multiplicador algoritmo
/*Salto de frecuencia 
con una señal FSK*/
fs=1e3;
Ts=1/fs;
win=2*%pi*50;
phi_in=0        //2*%pi/8;
T=2*%pi/(win); //un ciclo
N=20;        // Nro de ciclos de las señales


/////////// FSK - salto de fcia ////////////////
f1=47.5;  //f1 y f2 deben ser multiplo de 1/Tb
f2=52.5;
T1=1/f1;
T2=1/f2;
w1=2*%pi*f1;  //portadora 1
w2=2*%pi*f2;  //portadora 2     
wd=w2-w1;    //fcia de desviasión
Tb=(5*2*%pi)/(wd);    //duracion del bit
nc1=ceil(f1*Tb);  //nro de ciclos por cada bit
nc2=round(f2*Tb);
// Genero el mensaje banda base
m=[0 1];   
//m=[0 1 0 0 1 0 1 0 0 1 1 1 0 1 1 1 0 1 0 1]
n=length(m);
kc=round(T1/Ts);  //1 ciclo en discreto
N=n*nc1;  //total de ciclos en la ventana

//para graficar el mensaje
for i=1:1:(n+1)
    td(1,i)=(i-1)*Tb;
end
a=m.*ones(1,n); //señal digital
a(1,n+1)=m(1,n);


// Genero la señal FSK salto de frecuencia
vout=[];
phi0=[]; 
t1=[];
taux=[];
vref=[];        
  for i=1:1:(n)
    if a(1,i)== 0 then
        taux=(i-1)*Tb:T1/nc1:i*(Tb);
    else   
         taux=(i-1)*Tb:T2/nc2:i*(Tb);
    end     
    y=cos((w1+wd*a(1,i))*taux+phi_in);
    t1=[t1 taux];
    vref=[vref y];
    vf=length(t1);     //valor final
    end
  
kf=length(t1); //Para el algoritmo

subplot(2,1,1)
title('Señal banda Base- Mensaje')
plot2d2(td, a)
subplot(2,1,2)
xlabel("[seg]"), xgrid(color("gray"))
title('Señal FSK')
plot2d(t1, vref)
xlabel("[seg]"), xgrid(color("gray"))

xdel(0);
figure(0);clf;
subplot(2,1,1)
title('Señal FSK')
plot(t1, vref)
subplot(2,1,2)
Fasein=win*t1+phi_in;
title('fase de entrada vs fase de salida')
plot(t1, Fasein)
/////////////////////////////////////////////////
clc

disp('********* Inicio de varible ***********');
v0=0;
fase0=0;
wL=2*%pi*50;
//kc=T/Ts  //1 ciclo en discreto
disp('definición el punto de operación');
Pma=0  

disp('********* Historias ***************');
disp(' instante k-1          k ');
disp('***********************************');
Dw_k_1=0;    //sal del compensador en t=k-1
//Dw_k=0;     //sal del compensador en t=k-1
P_k_1=0;    //producto, entrada al filtro en t=k-1
Pm_k=0;
Pm_k_1=0;    //sal del filtro en t=k-1
Em_k=0;
Em_k_1=0;    //entrada al compensador en t=k-1

for k= 1:1:2000//kf//N*kc
disp('***************************************************************');
disp('muestra de la entrada en el instante k');
        k
disp('***************************************************************');
       t=t1(1,k)
       phin=win*t
       
disp('*********  Salida del multiplicador p ***************');
        P_k=vref(1,k)*v0 //multiplico las señales, sal del mult en k
   
disp('*********  salida del filtro ***************');
disp('ec en diferencia del filtro ');
disp('Pm_k= 0.0618551*P_k_1+ 0.9690724*Pm_k_1');
        a1=0.0579437;
        a2=0.9420563;
       Pm_k=a1*P_k_1+a2*Pm_k_1   

disp('**********Error medio= Pm*-Pm(k)******** ');
// Em_k=Pma- Pm_k_1
        Em_k=Pma - Pm_k
               
disp('*********  salida del compensador ***************');
disp('%ec en diferencia del compensador en t=k');
disp('Dw_k= 63.6619772*Em_k - 63.4619772*Em_k_1 + Dw_k_1');
        b1= -118.66794;
        b2=119.38052;
        Dw_k= b1*Em_k_1 + b2*Em_k + Dw_k_1
       
disp('*********  genero la señal vo ***************');
       w0= Dw_k+wL
       fase0= fase0+(w0*Ts)  //integro posible desborde
       v0=sin(fase0) //genero la señal en el intante k

disp('*********  actualizo las historias ***************');
        P_k_1=P_k
        producto(1,k)=P_k;
        Pm_k_1=Pm_k
        pmedio(1,k)=Pm_k;
        Em_k_1=Em_k
        Errorm(1,k)=Em_k;
        Dw_k_1 = Dw_k
        Deltaw(1,k)=Dw_k;
    
        subplot(2,1,1)
        plot(k*Ts, v0,'*r')
        subplot(2,1,2)
        plot(k*Ts, fase0,'*r')
        
       
end

figure(3);clf;
 subplot(2,1,1)
 title('Error ')
 plot(t1(1,1:k), Errorm)
 xgrid(color("gray")), xlabel("[seg]")
 subplot(2,1,2)
 title('Delta w - salida del comparador')
 plot(t1(1,1:k), Deltaw,'r')
 xgrid(color("gray")), xlabel("[seg]")


figure(4);clf;
 subplot(2,1,1)
 title('Producto- sailda Multipicador ')
 plot(t1(1,1:k-1), producto)
 xgrid(color("gray")), xlabel("[seg]")
 subplot(2,1,2)
 title('Producto medio - salida del filro')
 plot(t1(1,1:k-1), pmedio,'r')
 xgrid(color("gray")), xlabel("[seg]")
