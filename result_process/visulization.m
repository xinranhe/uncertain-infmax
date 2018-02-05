graphNames = {'Kron_CP_V512_E1024_getAll_NoCELF'};
resultFolder = '../result/';

for gi = 1:size(graphNames,2)
    expName = graphNames{gi};
    resultMtx = load(['../result/', expName,'_processed.csv']);

    rn = size(resultMtx,1)
    cn = size(resultMtx,2)

    YValues = [0.01,0.02,0.05,0.1];
    XValues = [0.01, 0.05, 0.1, 0.2, 0.5];

    XLabels = {'1%','5%','10%','20%','50%'};
    XLabels2 = {'1%','5%','10%','20%','50%','InfMax'};

    plotMtx = zeros(5,4,3);

    plot2Mtx = zeros(6,4);

    for ri = 1:rn
        edgeI = find(YValues==resultMtx(ri,1));
        detI = find(XValues==resultMtx(ri,2));
        % max
        plotMtx(detI, edgeI ,1) = resultMtx(ri, 18); 
        % Min
        plotMtx(detI, edgeI, 2) = resultMtx(ri, 21);
        % Obv
        plotMtx(detI, edgeI, 3) = resultMtx(ri, 24);

        plot2Mtx(detI, edgeI) = resultMtx(ri, 6);
        plot2Mtx(6, edgeI) = resultMtx(ri, 5);

    end

    x0=0;y0=0;width=520;height=500;

    stx = 0.5;
    sty = 0.5;
    edx = 5;
    edy = 6;
    planeColor = [0,1,0];
    planeAlpha = 0.7;
    tickFontSize = 15;
    planeZ = 1 - (1/ exp(1))

    isDrawPlane = false;

    % value compare graph
    h = figure;
    set(gcf,'units','points','position',[x0,y0,width,height])
    axis([0,5,0,6]);
    h1 = bar3(plot2Mtx, 0.3), hold on;
    set(gca,'XTickLabel',YValues);
    set(gca,'YTickLabel',XLabels2);
    figureHandle = gcf;
    set(findall(gcf,'type','axes'),'fontsize',tickFontSize);
    set(findall(gcf,'type','axes'),'fontWeight','bold')
    saveTightFigure(h,[resultFolder,expName,'_InfDiff.png']);
    close all;

end
