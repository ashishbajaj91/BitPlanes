function [ new_coords ] = InverseWarpCoords( box, A, image_size)
size_rows = image_size(1);
size_cols = image_size(2);

box(2,:) = box(2,:) - (size_cols+1)/2; 
box(1,:) = box(1,:) - (size_rows+1)/2; 

A = A^-1; A=A/A(9);

new_coords = A*box;

new_coords(1,:)= new_coords(1,:)./new_coords(3,:);
new_coords(2,:)= new_coords(2,:)./new_coords(3,:);

new_coords(2,:) = new_coords(2,:) + (size_cols+1)/2; 
new_coords(1,:) = new_coords(1,:) + (size_rows+1)/2; 

end