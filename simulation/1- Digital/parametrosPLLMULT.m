%Parametro de PLL Mutiplicador

%Ganancia derivada de la salida del mult
% respecto de la diferencia de fase
K=0.5;   %Ganancia para un punto de operación delta phi=0

%compensador
wz=2*pi*5.969;  %cero del compensador
tauz=1/wz;
kc=712.57;  %ganancia del compensador

%filtro
wc=2*pi*59.69; 
tauc=1/wc;