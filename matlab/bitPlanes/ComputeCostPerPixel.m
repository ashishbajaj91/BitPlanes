function [ cost ] = ComputeCostPerPixel( pix1, pix2 )
    cost = 0;
    %compute cost for bit planes
    for i=1:8 
        cost = cost + abs(pix1(i) - pix2(i));
    end
end

