function [I, Iref] = padimages(I, Iref)
    [h,w]=size(I); 
    % pad images with nan so later can determine valid regions
    hs=[1 1 1:h h h]; ws=[1 1 1:w w w]; I=I(hs,ws); Iref=Iref(hs,ws);
    hs=[1:2 h+3:h+4]; I(hs,:)=nan; Iref(hs,:)=nan;
    ws=[1:2 w+3:w+4]; I(:,ws)=nan; Iref(:,ws)=nan;
end