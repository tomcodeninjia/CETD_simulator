close all;
clear all;
clc;
MaxValue = 256;
dir1 = '.\frequency_3000count.csv';
dir2 = '.\frequency_shift_3000count.csv';
dir3 = '.\frequency_xorNonce_3000count.csv';
S = 21998:22020;
B_S = dec2bin(S);
s_1=S(1):5:S(length(S));
bs1=dec2bin(s_1,16);

M1=csvread(dir1);
M2=csvread(dir2);
M3=csvread(dir3);
%{
%find all pattern value in column 1 as x, store in C
A = [0,17,34,51,68, 85, 102, 119, 136, 153, 170, 187,204, 221, 238, 255];
X = 0:255;
%B = A;
%Both no-pattern
B = setdiff(X,A);
D = B;
C = [];
for i=D
    for j=B
       C = [C i*256+j+1]; 
    end
end
M1 = M(C,2)/MaxValue;
scatter(C,M1,10,'o','b');
%Both pattern
hold on
C=[];
B = A;
for i=A
    for j=B
       C = [C i*256+j+1]; 
    end
end
M1 = M(C,2)/MaxValue;
scatter(C,M1,5,'fill','r');
%pattern and no-pattern
hold on
C=[];
B = setdiff(X,A);
for i=A
    for j=B
       C = [C i*256+j+1]; 
    end
end

for i=B
    for j=A
       C = [C i*256+j+1]; 
    end
end
M1 = M(C,2)/MaxValue;
scatter(C,M1,5,'v','g');
%use C as x M(C,2) as y to draw the bar
legend('Both no-pattern','Both pattern', 'Pattern and no-pattern')




%plot(C,M1,'*');
scatter(C,M1,5,'fill','g');

%plot(M(:,1),M(:,2)/MaxValue);
%scatter(M(:,1),M(:,2)/MaxValue,5,'fill');
xlabel('Decimal Value of Input Message Block');
%}

set (gcf,'Position',[400,100,1000,400]);

bar(S,[M1(S+1,2)/MaxValue M2(S+1,2)/MaxValue M3(S+1,2)/MaxValue]);
%{
hold on
bar(S,M2(S+1,2)/MaxValue);
hold on
bar(S,M3(S+1,2)/MaxValue);
%}
%text(x,y,'\leftarrow0101010111101110');
%set the x axis


legend('Only shift','Two rounds shuffle then shift','Xor the nonce')
set(gca,'XTick',s_1,'XTickLabel',bs1);
h = gca;
rotateticklabel(gca,'x',10);
%}
ylim([0 1]);

ylabel('No. of Distinct Tags/ Max No. of Distinct Tags');






