 clc, clear, xdel(winsid())
//Proyecto 2 PLL Multiplicador
//Analizando la estabilidad
s=%s;
fs=1e3;
Ts=1/fs;    //tiempo de muestreo
wmin=2*%pi*(50-2.5) //wmin = win-dw

//Rechazo a la componente 2win
//Cuando el PLL está enganchado 
//Rechazo a 20db (2wmin/wc)=(Rx/1)
Rx=10
wc=(2*wmin)/Rx //fica del corte del LPF

//Defino el pto de operación
//Pm*=0 suponiendo que la mult es cos x sin
K=0.5;

//Si wx=10*wz==> el tan(wx/wz)=pi/2
Phiz=%pi/2; //fase que agrega el cero en wx
MF=%pi/4;   //Margen de Fase


wx=(tan(-(MF-Phiz)))*(wc)        //AB
wz=wx/10            //porque tan(wx/wz)=pi/2
kc=(wx*wz)/K       //ganancia del compensador

// compensador
Gc=syslin('c', kc*(1+s/wz), s)
  
//Transferencia del filtro LPF, 1er orden
Gf=syslin('c', 1, (1+s/wc))

//Transferencia GH(s)
GHs=(Gc*Gf)/s
zpk(GHs)

bode(GHs, 1e-1, 1e3,'rad')
title('GH polo doble error a la rampa en rp')
[MP, f_Hz]=p_margin(GHs)
AB=2*%pi*f_Hz

// Si achica wx para aumentar MF
//Recalculando
wx=32;
wz=wx/10            //porque tan(wx/wz)=pi/2
kc=(wx*wz)/K       //ganancia del compensador

// compensador
Gc=syslin('c', kc*(1+s/wz), s)
zpk(Gc)  
//Transferencia del filtro LPF, 1er orden
Gf=syslin('c', 1, (1+s/wc))

//Transferencia GH(s)
GHs=(Gc*Gf)/s
zpk(GHs)

xdel(0);
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
