function TheBytesAvailableFcn(serial_port,evento)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here

persistent vectordatos;
persistent counter_aux;
persistent error_counter;
if isempty(error_counter), error_counter = 0; end;


if isempty(vectordatos) 
    disp 'Definiendo vectordatos....'
    vectordatos=[];
end

persistent count;
if isempty(count) 
    count=0;
end


while (serial_port.BytesAvailable ~= 0),
    while (serial_port.BytesAvailable == 0), end;
    tempflag=fread(serial_port,1,'char');
    if tempflag~='@',
        display(['(@)ERROR']);
    else,
        while (serial_port.BytesAvailable == 0), end;
        tempflag=fread(serial_port,4,'char');
        if char(tempflag')~='Comm',
            display(['(Tmes)ERROR']);
        else,
            while (serial_port.BytesAvailable == 0), end;
            if fread(serial_port,1,'char')~='#',
                display(['(#)ERROR']);
            else,
                while (serial_port.BytesAvailable == 0), end;
                datos=fread(serial_port,2,'float');
                while (serial_port.BytesAvailable == 0), end;
                if fread(serial_port,1,'uint8')~='!',
                    display(['(!)ERROR']);
                else,
                        temp_prom=datos(1);
                        temp_inst=datos(2);
                        
 %                       if (datos(2)-counter_aux)~=1,
 %                           display(['(DATO ',num2str(datos(2)-counter_aux-1),')ERROR']);
 %                           error_counter=error_counter+1;
 %                       end;
 %                       counter_aux=datos(2);
                        
                        vectordatos=[vectordatos;[clock temp_prom temp_inst]];
                        if mod(count,10)==0, 
                            display(['T[ºC]:' num2str(temp_prom)]);  
							display(['UserPC:' num2str(temp_inst)]);  
                            save datosfile.mat vectordatos;
                            plotear
                            count = 0;     
                        end;
                    count = count+1;              

                end;
            end;
        end;
    end;
end;
end

