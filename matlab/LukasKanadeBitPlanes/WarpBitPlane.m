function [warpImage] = WarpBitPlane(I,H) 
    [h,w,l] = size(I);
    warpImage = I;
    for i = 1:l
        warpImage(:,:,i) = warpA(I(:,:,i),H,[h,w]);
    end
end