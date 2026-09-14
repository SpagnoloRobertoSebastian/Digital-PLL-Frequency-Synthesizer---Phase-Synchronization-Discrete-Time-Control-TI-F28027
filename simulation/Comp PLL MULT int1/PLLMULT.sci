clc, clear, xdel(winsid())
//Proyecto 2 PLL Multiplicador
//Analizando la estabilidad

fs=1e3;
Ts=1/fs;    // tiempo de muestreo
wc=2*%pi*5; //fica del corte del LPF
  

//Transferencia GH(s)
//válido para regimen permanente delta_phi=0
s=%s;
GHs=syslin('c', 1, 2*s*(1+s/wc));

//1° compensación polo en cero
Gc1=syslin('c', 1, s)
GHs1=Gc1*GHs;
zpk(GHs1)

bode(GHs1, 1e-1, 1e3,'rad')
title('GH polo doble error a la rampa en rp')
[MP, f_Hz]=p_margin(GHs1)
AB=2*%pi*f_Hz
 
// 2da compensación 
//Ajuste de ganancia, tal que wx
//este una decada por arriba
Gc2=syslin('c', 100, 1);         
GHs2=Gc2*GHs1;
zpk(GHs2)

figure(1); clf;
bode(GHs2, 1e-2, 1e3,'rad')
title('GH aumentando la ganancia')
[MP, f_Hz]=p_margin(GHs2)
 AB=2*%pi*f_Hz

//Compenso agregando un cero para
//obtener margen de fase > 45°
wx=AB;
wz=wx/10 
Gc3=syslin('c',(1+s/wz), 1);
GHs3=Gc3*GHs2;
zpk(GHs3)
  
figure(2); clf;
bode(GHs3, 1e-2, 1e3,'rad')
title('GH red de adelanto')
[MP, f_Hz]=p_margin(GHs3)
 AB=2*%pi*f_Hz
 

wz=wx/4 
Gc3=syslin('c',(1+s/wz), 1);
GHs3=Gc3*GHs2;
zpk(GHs3)
  
figure(3); clf;
bode(GHs3, 1e-2, 1e3,'rad')
title('GH red de adelanto')
[MP, f_Hz]=p_margin(GHs3)
 AB=2*%pi*f_Hz
 
//descritización de GH
 z=%z;
 GHz= ss2tf(dscr(tf2ss(GHs3), Ts));
 zpk(GHz)
 
 //descritización Compensador  
 Gc= Gc1*Gc2*Gc3
 zpk(Gc)
 Gcz= ss2tf(dscr(tf2ss(Gc), Ts));
 zpk(Gcz)
 

