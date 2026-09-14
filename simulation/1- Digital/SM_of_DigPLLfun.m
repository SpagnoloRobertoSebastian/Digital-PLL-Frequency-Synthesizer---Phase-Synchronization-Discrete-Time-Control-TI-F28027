function OutputVars = DigPLLfun(Input10V)
%% Codigo de la catedra
if (Input10V>10), Input10V=10;
elseif (Input10V<0), Input10V=0;
end;
InputPM1V = (Input10V/5.0)-1.0; %acondicionamiento de la entrada
UserLED=0;
UserGPIO=0;


%Codigo del alumno
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%inicialización de variables
Ts=0.001;
wL=2*pi*50;
pma=0;%pi/10;      %Definición el punto de operacion en rad

persistent SenialGenerada; if isempty(SenialGenerada), SenialGenerada=0; end;
persistent fase0; if isempty(fase0), fase0=0; end;

%Inicialización de las Historias
persistent P_k_1; if isempty(P_k_1), P_k_1=0; end;
persistent Pm_k_1; if isempty(Pm_k_1), Pm_k_1=0; end;
persistent Em_k_1; if isempty(Em_k_1), Em_k_1=0; end;
persistent Dw_k_1; if isempty(Dw_k_1), Dw_k_1=0; end;
   
          
    P_k = InputPM1V * SenialGenerada; % producto en el instante k

% Aplicar filtrado al producto de las seÃ±ales
   %   y(k)= 0.0579437 x(k-1) + 0.9420563 y(k-1)

   Pm_k = 0.0579437 * P_k_1 + 0.9420563* Pm_k_1;
   
   % Calculo del error de fase = pm*-pm(k)
   Em_k = pma - Pm_k;
   
   % Aplicar el compensador
   % y(k)=  -118.66794 x(k-1) + 119.38052  x(k) + y(k-1)

   Dw_k = -118.66794 * Em_k_1 +119.38052 * Em_k + Dw_k_1;
   
   % calculo de la fase de salida en el instante k
   w0= Dw_k + wL;
   fase0= fase0 + (w0*Ts);
    if( fase0 > 6.2831853) 
  
      fase0 = fase0- 6.2831853;
      end

   % Generar la señal de salida vo en el instante k
   SenialGenerada = sin(fase0);

   % Actualizacion de la historia de las variables
   P_k_1     = P_k;
   Pm_k_1    = Pm_k;
   Em_k_1    = Em_k;
   Dw_k_1    = Dw_k;

            
% 		// Output definition
       OutputPM1V = SenialGenerada;
       deltaOmega=(Dw_k+25.0)*3.3/50.0;
%        OutputPM1V = cos(fase0);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Codigo de la catedra
Output3_3V = (OutputPM1V+1.0)*3.3/2.0;

if (Output3_3V>3.3), Output3_3V=3.3;
elseif (Output3_3V<0), Output3_3V=0;
end;
OutputVars=[Output3_3V,fase0,Em_k,deltaOmega];

end

