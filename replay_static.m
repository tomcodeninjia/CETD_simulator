close all;
clear all;
clc;
MaxValue = 256;

dir1 = './results/cetd/';
Files = dir([dir1,'*.csv']);

str2 = '_replay_flipGFcetd_gf1_.csv'
str3 = ''
NumFiles = length(Files);
filename = {};
b=[];
for i =1:NumFiles
    filename{i} = Files(i).name;
end
filename1 = sort_nat(filename);
static_replay1 = [];
for i=1:NumFiles
    a = filename1{i};
    M_live = csvread([dir1,a]);
    fm=[];
    fm = [fm;a];
    cetd_stat=[];
    max1 = max(M_live(:,2));
    cetd_stat = [cetd_stat;max1];
    min1 = min(M_live(:,2));
    cetd_stat = [cetd_stat;min1];
    var1 = var(M_live(:,2));
    cetd_stat = [cetd_stat;var1];
    aver1 = mean(M_live(:,2));
    cetd_stat = [cetd_stat;aver1];
    static_replay1 = [static_replay1,cetd_stat];
end

tag_size = [10,20,50,80,100,200,500,800,1000,2000,5000,8000,10000];
static_r = [tag_size;static_replay1];
dst_filename = 'replay_static_cetd.csv';
csvwrite(dst_filename, static_r);

x = tag_size;
figure;
y = static_replay1(1,:);
subplot(2,2,1); plot(x,y);ylim([0 300]);
y = static_replay1(2,:);
subplot(2,2,2);plot(x,y);ylim([0 300]);
y = static_replay1(3,:);
subplot(2,2,3);plot(x,y);ylim([0 300]);
y = static_replay1(4,:);
subplot(2,2,4);plot(x,y);ylim([0 300]);
axes_handle = get(gcf, 'children');
axes(axes_handle(4)); title('Max Value');
axes(axes_handle(3)); title('Min Value');
axes(axes_handle(2)); title('Varity');
axes(axes_handle(1)); title('Average Value');


%annotation(gcf,'textbox','String',{'good'},'FontSize',30,'Position',[0.4 0.9 0.24 0.12],'edgecolor',get(gcf,'color'))

%{

static_replay1 = [];
M_live=csvread(origin);
cetd_stat=[];
max1 = max(M_live(:,2));
cetd_stat = [cetd_stat;max1];
min1 = min(M_live(:,2));
cetd_stat = [cetd_stat;min1];
var1 = var(M_live(:,2));
cetd_stat = [cetd_stat;var1];
aver1 = mean(M_live(:,2));
cetd_stat = [cetd_stat;aver1];

static_replay1 = [static_replay1,cetd_stat];

M_live=csvread(optv1);
D1=[];
max1 = max(M_live(:,2));
D1 = [D1;max1];
min1 = min(M_live(:,2));
D1 = [D1;min1];
var1 = var(M_live(:,2));
D1 = [D1;var1];
aver1 = mean(M_live(:,2));
D1 = [D1;aver1];
static_replay1 = [static_replay1,D1];

M_live=csvread(optv2);
D2=[];
max1 = max(M_live(:,2));
D2 = [D2;max1];
min1 = min(M_live(:,2));
D2 = [D2;min1];
var1 = var(M_live(:,2));
D2 = [D2;var1];
aver1 = mean(M_live(:,2));
D2 = [D2;aver1];
static_replay1 = [static_replay1,D2];

M_live=csvread(optv3);
D3=[];
max1 = max(M_live(:,2));
D3 = [D3;max1];
min1 = min(M_live(:,2));
D3 = [D3;min1];
var1 = var(M_live(:,2));
D3 = [D3;var1];
aver1 = mean(M_live(:,2));
D3 = [D3;aver1];
static_replay1 = [static_replay1,D3];

M_live=csvread(optv4_1);
D5=[];
max1 = max(M_live(:,2));
D5 = [D5;max1];
min1 = min(M_live(:,2));
D5 = [D5;min1];
var1 = var(M_live(:,2));
D5 = [D5;var1];
aver1 = mean(M_live(:,2));
D5 = [D5;aver1];
static_replay1 = [static_replay1,D5];

M_live=csvread(optv4_2);
D6=[];
max1 = max(M_live(:,2));
D6 = [D6;max1];
min1 = min(M_live(:,2));
D6 = [D6;min1];
var1 = var(M_live(:,2));
D6 = [D6;var1];
aver1 = mean(M_live(:,2));
D6 = [D6;aver1];
static_replay1 = [static_replay1,D6];

dst_filename = 'replay_static_1000.csv';
csvwrite(dst_filename, static_replay1);
%}
