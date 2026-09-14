clc, clear, xdel(winsid());

//PLL multiplicador algoritmo
/*Señal de entrada fcia cte
y Señal de entrada FM*/

fs=1e3;
Ts=1/fs;
win=2*%pi*50;
phi_in=0;//2*%pi/8;
T=2*%pi/(win); //un ciclo
N=50;        // Nro de ciclos de las señales

////////////  Señal de entrada ////////////////
 /*t1=0:Ts:N*T;
kf=length(t1);  //1 ciclo en discreto
kc=T/Ts;
vref=1*cos(win*t1+phi_in);

subplot(2,1,1)
title('señal vin vs señal vo')
plot(t1,vref)
xlabel("[seg]")
xgrid(color("gray"))
//Fase de entrada
subplot(2,1,2)
Fasein=win*t1+phi_in;
title('fase de entrada vs fase de salida')
plot(t1, Fasein)
xlabel("[seg]")
xgrid(color("gray"))
*/
///////////////////////////////////////////////

//////////      Señal FM       /////////////////
wm=2*%pi*7;   //2
wc=2*%pi*50;
wd=2*%pi*(2.5);
b=wd/wm;                //Indice de modulación
t1=0:Ts:N*T;
vm=1*sin(wm*t1+phi_in);   //modulante
vref=1*cos(wc*t1+b*vm);  //señal FM
kc=T/Ts  //1 ciclo en discreto

subplot(2,1,1)
title('señal FM vs señal vo')
plot(t1,vref)
xlabel("[seg]")
xgrid(color("gray"))
//Fase de entrada
subplot(2,1,2)
Fasein=win*t1+phi_in;
title('fase de entrada vs fase de salida')
plot(t1, Fasein)
xlabel("[seg]")
xgrid(color("gray"))
*/

/////////////////////////////////////////////////
clc

disp('********* Inicio de varible ***********');
v0=0;
fase0=0;
wL=2*%pi*50;
//kc=T/Ts  //1 ciclo en discreto
disp('definición el punto de operación');
Pma=0;//%pi/2; //0;  

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

for k= 1:1:N*kc//(kf-1)//
disp('***************************************************************');
disp('muestra de la entrada en el instante k');
        k
disp('***************************************************************');
       //t=t1(1,k)
       muestra_in=vref(1,k)
       //phin=win*t
       
disp('*********  Salida del multiplicador p ***************');
        P_k=muestra_in*v0 //multiplico las señales, sal del mult en k
   
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
 plot(t1(1,1:k), producto)
 xgrid(color("gray")), xlabel("[seg]")
 subplot(2,1,2)
 title('Producto medio - salida del filro')
 plot(t1(1,1:k), pmedio,'r')
 xgrid(color("gray")), xlabel("[seg]")
