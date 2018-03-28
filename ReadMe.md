# 基于声纹识别的 Windows Hello 身份验证应用

@(VPR)[声纹识别|身份验证|算法|论文|专利|大学本科毕业设计]

**Windows Hello VPR**是一种基于Windows平台下的身份验证应用，其利用现代人工智能热潮下的数据亮点：“语音”和“图像”两个方向中的语音信号，通过特征参数的提取与模式识别达到机器视觉的效果。特点概述：
 
- **使用方便** ：通过预先设置的语音模型，只要用户开机后说出预先设置的密钥即可识别登陆；
- **识别性强** ：通过大量的特征参数进入高斯混合模型数据模型演算建模，识别率高；
- **减少人力** ：省去了手动打入密码验证的服务，甚至可以不用在windows中设置多个用户；
- **优化办公** ：在工作过程的任何过程中，如果随机离开也可以防止电脑被别人盗窃数据；

-------------------

## Windows Hello 应用介绍

> Windows Hello是一种生物特征授权方式，让你可以实时访问自己的Windows 10设备。有了Windows Hello，用户只需要露一下脸，动动手指，就能立刻被运行Windows10的新设备所识别。Windows Hello不仅比输入密码更加方便，也更加安全。—— [Windows Hello](https://www.microsoft.com/zh-cn/windows/windows-hello)


作者：张立飞
联系方式：见 [Github用户Profile](https://github.com/leafspace)
开始时间：2017年6月25日

**如果您正在阅读这份文档，那么请注意如下几点：** <br/>
1.本程序存放在 [Github Windows-Hello-VPR](https://github.com/leafspace/Windows-Hello-VPR)，允许任何人fork和download，也允许任何人对本程序进行修改。但关于部分申报软件著作权和部分专利相关内容请不要用于商业用途。如需使用，请提前跟作者联系。<br/>
2.本程序允许读者在互联网内传播，欢迎任何人在Github或在`leafspace_zhang@163.com`邮箱内留言回馈。<br/>
3.本程序在研究和编码阶段不更新本文档，预计至2018年4月更新文档及系统详细算法内容。<br/>
4.本程序在研究和编码过程中，在代码段中依然有注释文档，用户可以依照注释信息进行学习。<br/>

**更新在2017年9月27日** <br/>
跟毕业设计指导老师交流后，准备做出如下修改考虑：<br/>
1.由于演示中可能会出现各种各样的问题，需要将具体的错误信息保存下来。现启用log的方式：
- **本地文档日志** ：在运行过程中，将系统的操作流程全部保存下来，用于错误时的分析。
- **服务器文档日志** ：在运行过程当中，通过预先设置的对服务器的设置，将运行时的错误信息，跟识别错误时的语音数据发送至服务器。

2.因为声音可能被其他人通过手机或其他设备录下来，然后在识别，缺少类似人脸识别中的活体检测，那么该如何考虑这方面的因素。

**更新在2017年10月9日** <br/>
识别效率提高修改考虑：<br/>
当前识别过程为：将目标语音与库模型进行概率匹配，概率最高即为识别目标。
若将登陆用户的语音放入登陆用户的模型中，计算出概率绝对值。将此数值作为标准值，为其创建高门限和低门限。当识别时，将目标语音放入登陆用户的模型中，计算出新贬值，如果新贬值在高门限与低门限中间，则规定其为登陆用户，允许其登入系统。
> 该算法已实现，在info.conf中可以设置method的数值来控制是否在用此方法来进行最后判定

**更新在2017年12月28日** <br/>
论文结构：<br/>
>>1.	开发背景与开发意义<br/>
>>2.	需求分析<br/>
>>3.	开发工具和开发技术<br/>
>>4.	系统设计<br/>
>>5.	系统知识<br/>
>>>1.	语音信号预处理<br/>
>>>> 1)	分帧和加窗<br/>
>>>> 2)	端点检测<br/>
>>>> 3)	预加重<br/>
>>>2.	语音信号特征参数提取<br/>
>>>> 1)	短时过零率<br/>
>>>> 2)	短时帧能量<br/>
>>>> 3)	Mel频率倒谱系数<br/>
>>>3.	训练模型建立<br/>
>>>4.	声纹识别<br/>
>>>5.	桌面应用<br/>
>>>6.	后台应用<br/>

**更新在2018年03月09日** <br/>
添加安装部署程序便于用户部署使用<br/>
答辩流程：<br/>
1. 中期检查（系统里填写）<br/>
2. 给老师看系统、看论文（修改几次）<br/>
3. 提交到查重系统（重复率超过百分之30的修改完再提交一遍，还超过，只能二辩）<br/>
4. 提交定稿并打印给系统和导师<br/>
5. 导师给分，并飞快交给学院，学院分给答辩组的某个老师评阅<br/>
6. 评阅老师给分数<br/>
7. 答辩组给分（系统也要拿去准备着查看）<br/>
8. 按照答辩组和导师的指点，再修改、再提交、再打印<br/>
9. 将系统、论文、说明文档提交给导师，并在规定的时间提交给图书馆<br/>
10. 离校<br/>

计算机学院毕业设计（论文）工作进度安排：<br/>
1. 2017年11月12日前	按选题结果，学院组织进行选题审核<br/>
2. 2017年11月24日前	完成选题论证工作<br/>
3. 2017年12月30日前	课题调研，文献查阅，完成开题报告<br/>
4. 2018年1月26日前	完成中期检查<br/>
5. 2018年4月21日前	完成论文检测以及答辩工作<br/>

-------------------
**更新在2018年03月23日** <br/>
下次版本功能更新：<br/>
1.实时录音结束功能<br/>
2.更新为加强版的录音头文件（支持多声道\多采样频率\多采样位数）<br/>
3.保存端点检测后的语音数据<br/>
4.加强log系统的输出内容<br/>

-------------------
**更新在2018年03月28日** <br/>
四月答辩具体时间安排：<br/>
答辩查重时间：2018年4月2日<br/>
完成各种资料时间：2018年4月9日<br/>
1)2018年4月12－13日，系主任进入系统分配评阅教师并及时通知评阅教师<br/>
2)2018年4月14－16日，评阅教师进入系统按要求完成评阅打分、评语等内容（评阅教师最晚在4月16日完成）<br/>
3)2018年4月17－19日，系主任进入系统分配答辩小组成员、答辩录入员等操作<br/>
4)2018年4月20日，教务办公布答辩安排表<br/>
<br/>
答辩程序:<br/>
1．答辩前，每位学生要向答辩老师递交一页a4纸的答辩提纲（包括本次毕业设计自己所做主要工作及成果），学生使用PPT电子稿进行答辩陈述。<br/>
2．每位学生答辩时间15—20分钟，其中10—15分钟由学生本人简要汇报毕业设计(论文)内容，其余时间回答答辩老师的提问，回答时要针对问题回答要领；若需要，可在黑板上或利用多媒体展开；如对问题不清楚时可请老师重述一遍。<br/>
3．答辩时，答辩小组除了对学生毕业设计(论文)内容提出质询外，还应考核有关的基本理论、计算方法、实验方法等，并做好记录，最终把答辩信息录入到“毕业设计（论文）智能管理系统”中。<br/>

**效果图片（2018年3月）** <br/>
![ScreenHost](https://github.com/leafspace/Windows-Hello-VPR/blob/master/%E5%8F%91%E5%B8%83%E9%83%A8%E7%BD%B2/%E6%95%88%E6%9E%9C%E8%A7%86%E9%A2%91/ScreenHost.png "效果图片")  
