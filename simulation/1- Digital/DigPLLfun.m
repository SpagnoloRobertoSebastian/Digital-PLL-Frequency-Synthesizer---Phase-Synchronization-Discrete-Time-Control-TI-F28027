function OutputVars = DigPLLfun(Input10V)
%% Codigo de la catedra
if (Input10V>10), Input10V=10;
elseif (Input10V<0), Input10V=0;
end;
InputPM1V = (Input10V/5.0)-1.0;
UserLED=0;
UserGPIO=0;

%Codigo del alumno
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 		// Matlab 50Hz Low-Pass Filter
% 				// >> s=tf('s');
% 				// >> c2d(1/(s/(2*pi*50)+1),1e-3)
% 				// Transfer function:
% 				//   0.2696
% 				// ----------
% 				// z - 0.7304
% 				// Sampling time: 0.001

            persistent InpZ1; if  size(InpZ1)==[0 0], InpZ1=0; end;
            persistent OutZ1; if  size(OutZ1)==[0 0], OutZ1=0; end;

			NumCoefZ1 = 0.2696;
			DenCoefZ1 = 0.7304;
			OutputZ0  = NumCoefZ1 * InpZ1 + DenCoefZ1 * OutZ1;
			OutZ1     = OutputZ0;
			InpZ1     = InputPM1V;
            
% 		// Saturation and Flag
			
			UserGPIO = 1;
			if      (OutputZ0>0.5),  OutputSat = 0.5;  
            elseif  (OutputZ0<-0.5), OutputSat = -0.5; 
            else,
                OutputSat = OutputZ0;
				UserGPIO  = 0;
            end;
            
% 		// Output definition
            OutputPM1V = OutputSat;




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Codigo de la catedra
Output3_3V = (OutputPM1V+1.0)*3.3/2.0;
if (Output3_3V>3.3), Output3_3V=3.3;
elseif (Output3_3V<0), Output3_3V=0;
end;
OutputVars=[Output3_3V,UserLED,UserGPIO];

end

