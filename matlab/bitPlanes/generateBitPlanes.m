function [census_transformed_image] = generateBitPlanes(img_gray)

    %img_gray = imread('test_image2.jpg');

    %Extract the size
    [m, n, k] = size(img_gray);

    %incase the image is not grayscale
    if k ~= 1
        img_gray = rgb2gray(img_gray);
    end

    %convert to double
    img_gray = im2double(img_gray);

    %Create a 8 channel image of same image size
    census_transformed_image = zeros(m-2, n-2, 8);

    %Comparison for census transform
    %234%
    %1%5%
    %876%

    % for 1st channel
    A = img_gray(2:m-1,1:n-2);
    B = img_gray(2:m-1,2:n-1);
    tmp = double((B - A) >= 0);
    census_transformed_image(:,:,1) = tmp;

    % for 2nd channel
    A = img_gray(1:m-2,1:n-2);
    B = img_gray(2:m-1,2:n-1);
    tmp = double((B - A) >= 0);
    census_transformed_image(:,:,2) = tmp;

    % for 3rd channel
    A = img_gray(1:m-2,2:n-1);
    B = img_gray(2:m-1,2:n-1);
    tmp = double((B - A) >= 0);
    census_transformed_image(:,:,3) = tmp;

    % for 4th channel
    A = img_gray(1:m-2,3:n);
    B = img_gray(2:m-1,2:n-1);
    tmp = double((B - A) >= 0);
    census_transformed_image(:,:,4) = tmp;

    % for 5th channel
    A = img_gray(2:m-1,3:n);
    B = img_gray(2:m-1,2:n-1);
    tmp = double((B - A) >= 0);
    census_transformed_image(:,:,5) = tmp;

    % for 6th channel
    A = img_gray(3:m,3:n);
    B = img_gray(2:m-1,2:n-1);
    tmp = double((B - A) >= 0);
    census_transformed_image(:,:,6) = tmp;

    % for 7th channel
    A = img_gray(3:m,2:n-1);
    B = img_gray(2:m-1,2:n-1);
    tmp = double((B - A) >= 0);
    census_transformed_image(:,:,7) = tmp;

    % for 8th channel
    A = img_gray(3:m,1:n-2);
    B = img_gray(2:m-1,2:n-1);
    tmp = double((B - A) >= 0);
    census_transformed_image(:,:,8) = tmp;
end
