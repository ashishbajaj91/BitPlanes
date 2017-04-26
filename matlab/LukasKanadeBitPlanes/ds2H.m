function [H,Hs] = ds2H( ds, wts )
    % compute homography from offsets ds
    Hs=eye(3); 
    Hs=Hs(:,:,ones(1,8));
    Hs(2,3,1)=wts(1)*ds(1);                       % 1 x translation
    Hs(1,3,2)=wts(2)*ds(2);                       % 2 y translation
    Hs(1:2,1:2,3)=eye(2)*wts(3)^ds(3);            % 3 scale
    Hs(2,1,4)=wts(4)*ds(4);                       % 4 shear
    Hs(1,1,5)=wts(5)^ds(5);                       % 5 scale non-uniform
    ct=cos(wts(6)*ds(6)); 
    st=sin(wts(6)*ds(6));
    Hs(1:2,1:2,6)=[ct -st; st ct];                % 6 rotation about z
    ct=cos(wts(7)*ds(7)); 
    st=sin(wts(7)*ds(7));
    Hs([1 3],[1 3],7)=[ct -st; st ct];            % 7 rotation about x
    ct=cos(wts(8)*ds(8)); 
    st=sin(wts(8)*ds(8));
    Hs(2:3,2:3,8)=[ct -st; st ct];                % 8 rotation about y
    H=eye(3); 
    for i=1:8, H=Hs(:,:,i)*H; end
end