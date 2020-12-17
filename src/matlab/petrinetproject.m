
% Start Safe n Sound by Clean Up Everything
clear;
close all;

% Get path from current file and generate absolute path
file_path = fileparts(mfilename('fullpath'));
img_path = strcat(file_path,"/../tex/img/");
dot_path = strcat(file_path,"/../dot/")

x0 = [1 1 1 0 0 0 0 0];

%% Transitions
syms t0 ton tnoff tnpe tccw tcw t1non t1off t1pe t2non t2off t2pe;
T = [t0 ton tnoff tnpe tccw tcw t1non t1off t1pe t2non t2off t2pe];

%% Places
syms Pon Pnoff Pnpe Pcw Pccw Pon Poff Ppe;
P = [Pon Pnoff Pnpe Pcw Pccw Pon Poff Ppe];

%% Transition
A = [-1 -1 -1  1  0  0  0  0;...
      0  0  0 -1  1  0  0  0;...
      0  0  0  1 -1  0  0  0;...
      0  0  0 -1  0  1  0  0;...
      0  0  0 -1  0  0  1  0;...
      0  0  0 -1  0  0  0  1;...
      0  0  0  0 -1  1  0  0;...
      0  0  0  0 -1  0  1  0;...
      0  0  0  0 -1  0  0  1;...
      1  0  0  0  0 -1  0  0;...
      0  1  0  0  0  0 -1  0;...
      0  0  1  0  0  0  0 -1]

filename = strcat(dot_path,'petrinet_tree.dot');
Ba = dotpetree(x0,A,5,filename);

T = [t0 ton tnoff tnpe tccw tcw t1non t1off t1pe t2non t2off t2pe];
