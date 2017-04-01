function M = LucasKanadeAffine(It, It1)

% input - image at time t, image at t+1 
% output - M affine transformation matrix

It = im2double(It);
It1 = im2double(It1);

% Image gradient
[X,Y] = meshgrid(1:size(It1,2),1:size(It1,1));
It_vect = reshape(It,numel(It),1);


% Intializing parameters
err_tol = Inf;              % Error tolerance initialized to Infinity
err_tol_Value = 1e-1;       % Desired tolerance 
deltaP = [0;0];
p = zeros(9,1);
k = 1;

% Iw = It1;
% Xw = X;
% Yw = Y;
[Ix1, Iy1] = gradient(It1);


while err_tol > err_tol_Vwalue
    params(:,k) = p;
    k = k+1;
    
    % Affine Transformation Matrix from paramters
    W = p2warp(p);
    
    % Affine transformation of the image
    % affine_warp(base-image,X,Y,warp-matrix, common-region) 
    [Iw, Xw, Yw, index] = affine_warp(It1,X,Y,W);
    
    Iw_vect = reshape(Iw,numel(Iw),1);
    Iw_vect(index) = [];
    
    It_vect_common = It_vect;
    It_vect_common(index) = [];
    
    err_im = -Iw_vect + It_vect_common;

    Ixw = affine_warp(Ix1,X,Y,W);
    Iyw = affine_warp(Iy1,X,Y,W);
    
    dW_dp = [];
    % Jacobian
    dW_dp(:,:,1) = [ reshape(Xw,numel(Xw),1), zeros(numel(Xw),1), reshape(Yw,numel(Yw),1)...
                        zeros(numel(Xw),1), ones(numel(Xw),1), zeros(numel(Xw),1)];
    dW_dp(:,:,2) = [ zeros(numel(Xw),1), reshape(Xw,numel(Xw),1), zeros(numel(Xw),1), ...
                        reshape(Yw,numel(Yw),1), zeros(numel(Xw),1), ones(numel(Xw),1)];
                    
    dTx = reshape(Ixw,numel(Ixw),1);
    dTx(index) =[];
    dTy = reshape(Iyw,numel(Iyw),1);
    dTy(index) = [];
    
    % Calculating steepest descent 
    sD = repmat(dTx,1,size(dW_dp,2)).*dW_dp(:,:,1) + repmat(dTy,1,size(dW_dp,2)).*dW_dp(:,:,2);
    
    H = [sum(sD(:,1).*sD(:,1)), sum(sD(:,1).*sD(:,2)), sum(sD(:,1).*sD(:,3)), sum(sD(:,1).*sD(:,4)), sum(sD(:,1).*sD(:,5)), sum(sD(:,1).*sD(:,6));
        sum(sD(:,2).*sD(:,1)), sum(sD(:,2).*sD(:,2)), sum(sD(:,2).*sD(:,3)), sum(sD(:,2).*sD(:,4)), sum(sD(:,2).*sD(:,5)), sum(sD(:,2).*sD(:,6));
        sum(sD(:,3).*sD(:,1)), sum(sD(:,3).*sD(:,3)), sum(sD(:,3).*sD(:,3)), sum(sD(:,3).*sD(:,4)), sum(sD(:,3).*sD(:,5)), sum(sD(:,3).*sD(:,6));
        sum(sD(:,4).*sD(:,1)), sum(sD(:,4).*sD(:,2)), sum(sD(:,4).*sD(:,3)), sum(sD(:,4).*sD(:,4)), sum(sD(:,4).*sD(:,5)), sum(sD(:,4).*sD(:,6));
        sum(sD(:,5).*sD(:,1)), sum(sD(:,5).*sD(:,2)), sum(sD(:,5).*sD(:,3)), sum(sD(:,5).*sD(:,4)), sum(sD(:,5).*sD(:,5)), sum(sD(:,5).*sD(:,6));
        sum(sD(:,6).*sD(:,1)), sum(sD(:,6).*sD(:,2)), sum(sD(:,6).*sD(:,3)), sum(sD(:,6).*sD(:,4)), sum(sD(:,6).*sD(:,5)), sum(sD(:,6).*sD(:,6))];

   err_im_ = reshape(err_im,numel(err_im),1);
   
    temp = sum(repmat(err_im_,1,size(sD,2)).*sD,1);
    
    deltaP = H\temp'; 
    
    err_tol = norm(deltaP);
    p = p + deltaP;
    
   
end

M = p2warp(p);
save('params.mat','params');

end

function[inv_p] = inv_of_warp(p)

inv_p = [ -p(1) - p(1)*p(4) + p(2)*p(3);
            -p(2);
            -p(3);
            -p(4) - p(1)*p(4) + p(2)*p(3);
            -p(5) - p(4)*p(5) + p(3)*p(6);
            -p(6) - p(1)*p(6) + p(2)*p(5)];
        
inv_p = inv_p./((1+p(1))*(1+p(4)) - p(2)*p(3));

end

function[new_p] = warp_op(p,dp)

new_p = [ p(1) + dp(1) + p(1)*dp(1) + p(3)*dp(2);
            p(2) + dp(2) + p(2)*dp(1) + p(4)*dp(2);
            p(3) + dp(3) + p(1)*dp(3) + p(3)*dp(4);
            p(4) + dp(4) + p(2)*dp(3) + p(4)*dp(4);
            p(5) + dp(5) + p(1)*dp(5) + p(3)*dp(6);
            p(6) + dp(6) + p(2)*dp(5) + p(4)*dp(6);];           
end

function[warp] = p2warp(p)

warp = [ 1+p(1) p(3) p(5);
            p(2) 1+p(4) p(6);
            0   0   1];

end

function[p] = warp2p(warp)

p = [warp(1)-1;
        warp(2);
        warp(4);
        warp(5)-1;
        warp(7);
        warp(8)];

end

function[rect] = common_region(im1,im2,W)
% Function where coordinates of im2 is warped using the transformation
% matrix W and the common region between im1 and warped im2 is calculated

coord1 = [ 1 ,1, 1;
            size(im1,2), 1 ,1;
            size(im1,2), size(im1,1), 1;
            1, size(im1,1), 1];
        
coord2 = [ 1 ,1, 1;
            size(im2,2), 1 ,1;
            size(im2,2), size(im2,1), 1;
            1, size(im2,1), 1];
        
trans_coord = W*coord2';
trans_coord = trans_coord';

x1 = max([coord1(1,1),coord1(4,1),trans_coord(1,1),trans_coord(4,1)]);
y1 = max([coord1(1,2),coord1(2,2),trans_coord(1,2),trans_coord(2,2)]);
x2 = min([coord1(2,1),coord1(3,1),trans_coord(2,1),trans_coord(3,1)]);
y2 = min([coord1(3,2),coord1(4,2),trans_coord(3,2),trans_coord(4,2)]);

rect = [ceil(x1), ceil(y1), floor(x2), floor(y2)];
end

function[Iw, varargout] = affine_warp(It,X,Y,W)

    warped_xy = W*[reshape(X,numel(X),1), reshape(Y,numel(Y),1), ones(numel(X),1)]';
    
    Xnew = reshape(warped_xy(1,:)',size(X,1),size(X,2));
    Ynew = reshape(warped_xy(2,:)',size(Y,1),size(Y,2));
    
    Iw = interp2(It,Xnew,Ynew);   
    
    index  = find(warped_xy(1,:)'<1);
    index = [index; find(warped_xy(1,:)' > size(It,2))];
    index = [index; find(warped_xy(2,:)' < 1)];
    index = [index; find(warped_xy(2,:)' > size(It,1))];
    
    Xw = reshape(X,numel(X),1);
    Xw(index) = [];
    Yw = reshape(Y,numel(Y),1);
    Yw(index) = [];
    
    if nargout > 1
        varargout{1} = Xw;
        varargout{2} = Yw;
        varargout{3} = index;
    end

end
