function [H,Ip] = LucasKanade2(I, Iref)

I = im2double(I);
Iref = im2double(Iref);

[XGra_Iref, YGra_Iref] = gradient(Iref);



end