clc, clear, xdel(winsid());

//PLL multiplicador algoritmo
/*Salto de frecuencia 
con una señal FSK*/

fs=1e3;
Ts=1/fs;
win=2*%pi*50;
phi_in=0;//2*%pi/8;
T=2*%pi/(win); //un ciclo
N=20;        // Nro de ciclos de las señales

disp('********* Inicio de varible ***********');
v0=0;
fase0=0;
wL=2*%pi*50;
vi=1;   //valor inicial
//kc=T/Ts  //1 ciclo en discreto
disp('definición el punto de operación');
Pma=0;  

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
////////////////////////////////////////////////

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
//m=[0 1 0 1 0 1 1 0];   
m=[0 1];
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
    
    ////////////inicio algoritmo//////////////
    for j=vi:1:vf
     P_k=vref(1,j)*v0;   //producto
     
     a1=0.0579437;
     a2=0.9420563;
     Pm_k=a1*P_k_1+a2*Pm_k_1;//ec del filtro
     
     Em_k=Pma - Pm_k;        //error medio
     
     b1=-118.66794;
     b2=119.38052;
     Dw_k= b1*Em_k_1 + b2*Em_k + Dw_k_1; //ec comp.
     
     //genero la señal
     w0= Dw_k+wL;
     fase0= fase0+(w0*Ts); //integro posible desborde
     v0=sin(fase0); //genero la señal en el intante
     vout(1,j)=v0;
     phi0(1,j)=fase0;
     
     //actualizo historias
     P_k_1=P_k;
    producto(1,j)=P_k;
    Pm_k_1=Pm_k;
    pmedio(1,j)=Pm_k;
    Em_k_1=Em_k;
    Errorm(1,j)=Em_k;
    Dw_k_1 = Dw_k;
    Deltaw(1,j)=Dw_k;
    ////////////fin algoritmo ////////////////
    end
    vi=j+1;
  end
  
subplot(2,1,1)
title('Señal banda Base- Mensaje')
plot2d2(td, a)
subplot(2,1,2)
xlabel("[seg]"), xgrid(color("gray"))
title('Señal FSK')
plot2d(t1, vref)
xlabel("[seg]"), xgrid(color("gray"))


figure(1);clf;
subplot(2,1,1)
title('Señal FSK')
plot(t1, vref)
plot(t1, vout, 'r')
subplot(2,1,2)
Fasein=win*t1+phi_in;
title('fase de entrada vs fase de salida')
plot(t1, Fasein)
plot(t1, phi0, 'r')

figure(2);clf;
subplot(2,1,1)
title('salida multiplicador')
plot(t1,  producto)
subplot(2,1,2)
title('Salida del filtro')
plot(t1, pmedio)

figure(3);clf;
subplot(2,1,1)
title('Error de fase')
plot(t1, Errorm)
subplot(2,1,2)
title('Salida del Compensador')
plot(t1, Deltaw)

