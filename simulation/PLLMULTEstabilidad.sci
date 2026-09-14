clc, clear//, xdel(winsid())
//Proyecto 2 PLL Multiplicador
//Analizando la estabilidad
s=%s;
fs=1e3;
Ts=1/fs;    // tiempo de muestreo
wmin=2*%pi*(50-2.5) //wmin = win-dw


//Rechazo a la componente 2win
//Cuando el PLL está enganchado 
//Rechazo a 20db (2wmin/wc)=(Rx/1)
Rx=10
wc=(2*wmin)/Rx //fica del corte del LPF
wx=wc;
//Defino el pto de operación
//Pm*=0
dphi0=0.5; //ganancia en el pto de operacion

//Parametros del compensador
wz=wx/10
kc=2*wx*wz
Gc=syslin('c', kc*(1+s/wz), s)
  
//Transferencia del filtro LPF, 1er orden
Gf=syslin('c', 1, (1+s/wc));

//Transferencia GH(s)
GHs=(dphi0*Gc*Gf)/s;
//GHs=syslin('c',4166.48*(s+6.9802) , s^2*(s+59.6903));
zpk(GHs)

bode(GHs, 1e-1, 1e3,'rad')
title('GH polo doble error a la rampa en rp')
[MP, f_Hz]=p_margin(GHs)
AB=2*%pi*f_Hz
 
//Como la fs es >> a la Fin, no es necesario aplicar 
//la transformación binilial ya que estoy en la zona lineal de la curva
 //descritización Filtro de 1° orden  
 z=%z;
 Gfz= ss2tf(dscr(tf2ss(Gf), Ts))
 zpk(Gfz)

  //descritización Compensador  
 Gcz= ss2tf(dscr(tf2ss(Gc), Ts))
 zpk(Gcz)
