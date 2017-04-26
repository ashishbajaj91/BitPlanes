function [] = DrawImage(I, rect, H)
    box = rect2box(rect);
    quad = [box';ones(1,5)];
    newquad = InverseWarpCoords( quad, H, size(I));
    imshow(I); hold on;
    plot(newquad(2,:),newquad(1,:),'b','LineWidth',2);
    drawnow;
end