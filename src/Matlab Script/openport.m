function openport(varargin)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

if nargin==0,
   COM='COM999';
else,
    COM=varargin(1);
end;

serial_port = instrfind;
for i=1:length(serial_port), fclose(serial_port(i)); end; clear i;
%serial_port = serial('COM3','BaudRate',9600);
serial_port = serial(COM,'BaudRate',114636); %115200
%s = serial('COM102','BaudRate',460800);
fclose(serial_port);
set(serial_port,'InputBufferSize',10000)
% set(serial_port,'BytesAvailableFcnCount',1)
% set(serial_port,'BytesAvailableFcnMode','byte')
% lalala='hola ';
% set(serial_port,'BytesAvailableFcn',{'display([lalala])'})


    

set(serial_port,'BytesAvailableFcnCount',15);
set(serial_port,'BytesAvailableFcnMode','byte')
set(serial_port,'BytesAvailableFcn',{@TheBytesAvailableFcn})

fopen(serial_port);

vectordato1_para_grafico=[];
vectordato2_para_grafico=[];



