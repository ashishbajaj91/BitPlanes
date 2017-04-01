function [u,v] = LucasKanadeInverseCompositional(It, It1, rect)
% input - image at time t, image at t+1, rectangle (top left, bot right coordinates)
% output - movement vector, [u,v] in the x- and y-directions.

It = im2double(It);
It1 = im2double(It1);

% Image template
template = It(rect(2):rect(4),rect(1):rect(3));
% Image gradient
[Itx, Ity] = gradient(It);
% Template gradient
deltaTx = Itx(rect(2):rect(4),rect(1):rect(3));
deltaTy = Ity(rect(2):rect(4),rect(1):rect(3));
dW_dp = eye(2); % Partial difference of Warp with parameters p

H = [sum(sum(deltaTx.^2)), sum(sum(deltaTx.*deltaTy));
        sum(sum(deltaTx.*deltaTy)), sum(sum(deltaTy.^2))];
    
% Intializing parameters
err_tol = Inf;              % Error tolerance initialized to Infinity
err_tol_Value = 1e-5;       % Desired tolerance 
deltaP = [0;0];
p = [0;0];

[X,Y] = meshgrid(1:size(It,2),1:size(It,1));

while err_tol > err_tol_Value
    
    Xnew = X+p(1);
    Ynew = Y+p(2);
    I_new = interp2(X,Y,It1,Xnew,Ynew);
    err_im = I_new(rect(2):rect(4),rect(1):rect(3)) - template;
    deltaP = H\[sum(sum(deltaTx.*err_im)); sum(sum(deltaTy.*err_im))]; 
    err_tol = norm(deltaP);
    p = p - deltaP;
   
end

u = (p(1));
v = (p(2));

end 