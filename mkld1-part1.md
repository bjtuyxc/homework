How did you encode the outputs?
用一个长度为4的向量(实质上长度为5，只不过从下标1开始使用)，分别对应pose的[straight, left, right, up]，利用0或1表示该特征是否存在，如：图片的pose为straight，则向量为[1,0,0,0]

Change the parameters and retry the experiments: the number of hidden units, learning rate and momentum, the number of epochs, the initial weights, etc. What was the maximum classification accuracy achieved on the training set?
经过多次测试，隐藏层为8，learning rate为0.1，momentum为0，epochs为200，随机化初始weights，训练集上的最大分类准确率为100%

How many epochs did it take to reach this level?
26次，不过第24次就能达到99%

How about for the validation and test set?
90.6475% 和 94.2308%

Draw the curves for the experiment outputs. We suggest you put trainperf, t1perf and t2perf in one figure, and trainerr, t1err and t2err in another. The x-axis denotes the number of epochs and the y-axis denotes the corresponding outputs.
这个没有画啦，没时间弄这个了

Now, try taking a look at how backpropagation tuned the weights of the hidden units with respect to each pixel. Use hidtopgm and outtopgm to visualize the weights of hidden units and the output units. The lowest weights are mapped to pixels with the value of zero (in terms of grayscale), and the highest mapped to 255. Use the PGM viewer program to display the weights. What kind of conclusions can you draw from the weights learned? 

输入层到隐藏层的权重图片：

![image-20180426232626953](https://ws2.sinaimg.cn/large/006tNc79ly1fqqhq40fccj307606qjs9.jpg)

![image-20180426232642248](https://ws1.sinaimg.cn/large/006tNc79ly1fqqhq2hbonj307606szl5.jpg)

![image-20180426232655157](https://ws1.sinaimg.cn/large/006tNc79ly1fqqhq05ztwj307606m3ze.jpg)

![image-20180426232706326](https://ws3.sinaimg.cn/large/006tNc79ly1fqqhq68iasj307606m3ze.jpg)

![image-20180426232723180](https://ws3.sinaimg.cn/large/006tNc79ly1fqqhq5b03pj307806owfa.jpg)

![image-20180426232735094](https://ws2.sinaimg.cn/large/006tNc79ly1fqqhpzpelmj307606mt9h.jpg)

![image-20180426232748391](https://ws1.sinaimg.cn/large/006tNc79ly1fqqhpz24orj307206qmy0.jpg)

![image-20180426232832005](https://ws1.sinaimg.cn/large/006tNc79ly1fqqhq5smp7j307006q754.jpg)

![image-20180426232842398](https://ws1.sinaimg.cn/large/006tNc79ly1fqqhq1ll29j307606ojs8.jpg)

隐藏层到输出层的权重图片：(放大500倍)

![image-20180426233153022](https://ws1.sinaimg.cn/large/006tNc79ly1fqqhq156zpj305400iglh.jpg)

![image-20180426233054703](https://ws1.sinaimg.cn/large/006tNc79ly1fqqhq21pjbj305800mjr9.jpg)

![image-20180426233253344](https://ws4.sinaimg.cn/large/006tNc79ly1fqqhq4tyr3j305000ka9x.jpg)

![image-20180426233330773](https://ws3.sinaimg.cn/large/006tNc79ly1fqqhq2z4pgj304y00oq2t.jpg)



结论：（不知道对不对）

可以看出从输入层到隐藏层的权重图片中，正中间可以看出一个人形的轮廓，其中白色点多集中在人形轮廓中（也就是255的点），说明从输入层到隐藏层对原图中人形部分的地方赋予了较大的权重，主要学习到人形的轮廓

从隐藏层到输出层的权重图片中，图片中的颜色差距很大，基本为黑色和白色为主，说明这一部分中，主要是进行分类操作

Do the hidden units seem to weight particular regions of the image greater than others? 

是的，中间有一个人形轮廓的区域，且白色点主要集中在该区域



Do you encounter any difficulty in the experiments? 
第一次写完程序的时候，训练集合、测试集合、验证集合的准确率、误差全为0

How did you solve the problem? 
通过打断点进行调试，发现计算误差的时候，使用了int类型的变量保存，而output vector为double类型的，且范围为[0,1]，导致两个double类型的变量相减转化为int基本为0

Can you describe your findings during the experiment or anything interesting?
当隐藏层的单元的个数较大于输出层单元的个数时，训练数据总是能够拟合，训练数据准确率总能达到100%左右，验证和测试数据的准确率都能保持在90%之间，当隐藏层的单元个数小于输出层单元个数时，训练数据很难拟合，并且在训练的过程中loss可能会产生波动，可能出现上升或下降，训练数据和验证、测试数据的准确率很低