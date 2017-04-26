function [ warp_im ] = warpA( im, A, out_size )
% warp_im=warpAbilinear(im, A, out_size)
% Warps (w,h,1) image im using affine (3,3) matrix A 
% producing (out_size(1),out_size(2)) output image warp_im
% with warped  = A*input, warped spanning 1..out_size
% Uses nearest neighbor mapping.
% INPUTS:
%   im : input image
%   A : transformation matrix 
%   out_size : size the output image should be
% OUTPUTS:
%   warp_im : result of warping im by A
[size_rows, size_cols] = size(im);

[coord_x, coord_y] = meshgrid(1:out_size(2),1:out_size(1));

coord_x = coord_x - (size_cols+1)/2; 
coord_y = coord_y - (size_rows+1)/2;

X = coord_x(:);
Y = coord_y(:);

A = A^-1; A=A/A(9);

new_coords = A*[Y';X';ones(1,length(Y))];

new_coords(1,:)= new_coords(1,:)./new_coords(3,:);
new_coords(2,:)= new_coords(2,:)./new_coords(3,:);

new_coords_y = reshape(new_coords(1,:),size_rows, size_cols);
new_coords_x = reshape(new_coords(2,:),size_rows, size_cols);

warp_im = interp2(coord_x,coord_y, im, new_coords_x, new_coords_y);

%warp_im(isnan(warp_im)) = 0;
end