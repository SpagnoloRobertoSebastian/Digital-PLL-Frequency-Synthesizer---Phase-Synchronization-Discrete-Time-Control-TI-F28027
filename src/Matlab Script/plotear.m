function plotear()
set(gcf,'Name','PLACA 2','NumberTitle','off')
load('datosfile.mat')
plot(vectordatos(:,8),'r')
hold on;
plot(vectordatos(:,7))
hold off;
axis_aux=axis;
axis([axis_aux(1),axis_aux(2),0,150])
%axis([axis_aux(2)-20000,axis_aux(2),40,80])
grid on