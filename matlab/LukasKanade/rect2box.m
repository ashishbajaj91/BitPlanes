function[box] = rect2box(rect)
% function to generate coordinates for the bounding box given rect
box= [ rect(1) , rect(2);
        rect(1), rect(4);
        rect(3), rect(4);
        rect(3), rect(2);
        rect(1), rect(2);];
end