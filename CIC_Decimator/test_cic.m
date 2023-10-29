N=64;
i = (0:N-1);
Fs = 44.1e3;  

Hd = dsp.CICDecimator( ...
        'FixedPointDataType', 'Minimum section word lengths', ...
        'DecimationFactor', 1, ...
        'DifferentialDelay', 20, ...
        'NumSections', 4, ...
        'OutputWordLength', 128);
    
    
        %
        
        %'NumSections', 1, ...
    
info(Hd,'long');
nt = numerictype(1,64,0);
[WLs,FLs] = getFixedPointInfo(Hd,nt);
data = 0*i;
for iii = 1:64
   data(:,iii) =  fi(9223372036854775807, true, 64, 0); % 9223372036854775807 % 2147483646
end

datar = reshape(data, [N, 1]);
%data = fi(2147483646 * cos(2*pi*500/8000*i), true, 32, 0);
src = dsp.SignalSource(datar,64);
y = zeros(32,64);
for ii = 1:32
     y(ii,:) = Hd(src());   
end

D = Hd.DecimationFactor;
diffDelay = Hd.DifferentialDelay;
NumSect = Hd.NumSections;
gainCIC = ...
   (D*diffDelay)^NumSect;
stem(i(1:56)/Fs,double(data(4:59)));
hold on;     
stem(i(1:64)/(Fs/D),double(y(1, 1:end)),... %/gainCIC
    'r','filled');
xlabel('Time (sec)');
ylabel('Signal Amplitude');
legend('Original signal',...
    'Decimated signal',...
    'Location','north');
hold off;


% filtered = step(Hd,data);
% plot(s);
% hold on;
% %plot(data);
% hold off;
% figure();
% plot(filtered);

% figure();
% plot(y);


    
%s = fi(data,1,8,0,'RoundingMethod', 'Round', 'OverflowAction', 'Saturate');
%sr = reshape(s, [N, 1]);
%dr = reshape(data, [N, 1]);
%sr = reshape(s, [N/2, 2]);
%filtered = fi(step(Hd,sr),1,8,0,'RoundingMethod', 'Round', 'OverflowAction', 'Saturate');
%filtered = filter(Hd,sr);






























