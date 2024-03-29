// 不是我写的，参考博客：https://cloud.tencent.com/developer/article/1009051#:~:text=caffe.pr,%E4%B8%AD%E7%9A%84%E7%BB%93%E6%9E%84%E7%9B%B8%E5%AF%B9%E5%BA%94%E3%80%82


// syntax用来指定protobuf的版本
syntax = "proto2";

// package可以看作C++中的namespace，与Caffe C++代码中的namespace caffe对应
// package用来避免名称冲突
package caffe;


// 在消息定义中，每个字段都有唯一的一个数字标识符。这些标识符是用来在消息的二进制格式中识别各个字段的，一旦开始使用就不能够再改变。
// 注：[1,15]之内的标识号在编码的时候会占用一个字节。[16,2047]之内的标识号则占用2个字节。所以应该为那些频繁出现的消息元素保留 [1,15]之内的标识号。
// required：一个格式良好的消息一定要含有一个这种字段，表示该值是必须要设置的。
// optional：消息格式中该字段可以有0个或1个值（不超过1个）。
// repeated：在一个格式良好的消息中，这种字段可以重复任意多次（包括0次）。重复的值的顺序会被保留，表示该值可以重复，相当于Java中的List。


// Specifies the shape (dimensions) of a Blob.
// 指定Blob的shape，4-D shape
message BlobShape {
  //数据块形状定义为Num * Channel * Height * Wight, 原因在于caffe基于容器的多维嵌套来实现高维数据的封装, 即vector。 
  repeated int64 dim = 1 [packed = true];
}


// Blob数据块，包括Blob shape，数据和微分
message BlobProto {
  // Blob的shape, 即numpy中的shape
  optional BlobShape shape = 7;
  // Blob的数据部分
  repeated float data = 5 [packed = true];
  // Blob的微分部分
  repeated float diff = 6 [packed = true];
  // Blob中的数据部分(double类型)
  repeated double double_data = 8 [packed = true];
  // Blob的微分部分(double类型)
  repeated double double_diff = 9 [packed = true];

  // 4D dimensions -- deprecated.  Use "shape" instead.
  // Blob的4个维度，已被Blob shape代替
  // Blob中数据的个数(例如卷积核的个数)
  optional int32 num = 1 [default = 0];
  // Blob中数据的通道数
  optional int32 channels = 2 [default = 0];
  // Blob中数据的高度
  optional int32 height = 3 [default = 0];
  // Blob中数据的宽度
  optional int32 width = 4 [default = 0];
}


// The BlobProtoVector is simply a way to pass multiple blobproto instances around.
// BlobProtoVector, 用来保存多个BlobProb对象的Vector
message BlobProtoVector {
  repeated BlobProto blobs = 1;
}


//图像数据, channel-图像通道数, height-高度, width-宽度, data-图像像素数据, label-图像标签, float_data-图像浮点型数据(0-1之间), encoded-图像编码方式
message Datum {
  // 图像的通道数
  optional int32 channels = 1;
  // 图像的高度
  optional int32 height = 2;
  // 图像的宽度
  optional int32 width = 3;
  // the actual image data, in bytes
  // 实际的图像数据，以字节形式(uint8)表示
  optional bytes data = 4;
  // 图像对应的标签，必须为整形
  optional int32 label = 5;
  // Optionally, the datum could also hold float data.
  // 可选表示，图像数据表示为float数据，即0-255归一化到0-1之间
  repeated float float_data = 6;
  // If true data contains an encoded image that need to be decoded
  // encoded为true表示图像采用压缩表示，需要解码
  optional bool encoded = 7 [default = false];
}


// Filler参数, filler主要对网络权重进行初始化
// Filler类型分为常量初始化（constant）、高斯分布初始化（gaussian）、positive_unitball初始化、均匀分布初始化（uniform）、xavier初始化、msra初始化、双线性初始化（bilinear）
message FillerParameter {
  // The filler type.
  // Filler的类型
  optional string type = 1 [default = 'constant'];
  // 常量初始化的值
  optional float value = 2 [default = 0]; // the value in constant filler
  // 均匀分布初始化中的最小值
  optional float min = 3 [default = 0]; // the min value in uniform filler
  // 均匀分布初始化中的最大值
  optional float max = 4 [default = 1]; // the max value in uniform filler
  // 高斯分布初始化中的均值
  optional float mean = 5 [default = 0]; // the mean value in Gaussian filler
  // 高斯分布初始化中的标准差
  optional float std = 6 [default = 1]; // the std value in Gaussian filler
  // The expected number of non-zero output weights for a given input in
  // Gaussian filler -- the default -1 means don't perform sparsification.
  // 在高斯分布初始化中给定输入及权重，期望输出非0值，默认值-1表示不进行稀疏化
  optional int32 sparse = 7 [default = -1];
  // Normalize the filler variance by fan_in, fan_out, or their average.
  // Applies to 'xavier' and 'msra' fillers.
  // 通过fan_in, fan_out或average来归一化filler方差，主要应用到'xavier'和'msra' filler中
  enum VarianceNorm {
    FAN_IN = 0;
    FAN_OUT = 1;
    AVERAGE = 2;
  }
  // 定义filler方差归一化，默认为FAN_IN
  optional VarianceNorm variance_norm = 8 [default = FAN_IN];
}


//神经网络参数
message NetParameter {
  // 神经网络名字
  optional string name = 1; // consider giving the network a name

  // DEPRECATED. See InputParameter. The input blobs to the network.
  // 已废弃。网络的输入部分，具体请看InputParameter。
  repeated string input = 3;

  // DEPRECATED. See InputParameter. The shape of the input blobs.
  // 已废弃。输入blob的shape，具体请看InputParameter。
  repeated BlobShape input_shape = 8;

  // 4D input dimensions -- deprecated.  Use "input_shape" instead.
  // If specified, for each input blob there should be four
  // values specifying the num, channels, height and width of the input blob.
  // Thus, there should be a total of (4 * #input) numbers.
  // 已废弃。用input_shape代替。
  repeated int32 input_dim = 4;

  // Whether the network will force every layer to carry out backward operation.
  // If set False, then whether to carry out backward is determined
  // automatically according to the net structure and learning rates.
  // 网络中是否每一层都执行反向传播的标志，如果设为false，反向传播会根据网络结构和学习率自动进行。
  optional bool force_backward = 5 [default = false];

  // The current "state" of the network, including the phase, level, and stage.
  // Some layers may be included/excluded depending on this state and the states
  // specified in the layers' include and exclude fields.
  // 网络的当前状态，包括phase, level和stage，(phase应该是对应prototxt文件中的TRAIN,TEST)
  // 某些层是否included/excluded依赖于层中include，exclue字段指定的state。
  optional NetState state = 6;

  // Print debugging information about results while running Net::Forward,
  // Net::Backward, and Net::Update.
  // 在执行Net::Forward,Net::Backward, Net::Update时是否打印调试信息。
  optional bool debug_info = 7 [default = false];

  // The layers that make up the net.  Each of their configurations, including
  // connectivity and behavior, is specified as a LayerParameter.
  // 构成网络的layer，每一个layer的配置，包括连接性和行为都在LayerParameter中指定。
  repeated LayerParameter layer = 100;  // ID 100 so layers are printed last.

  // DEPRECATED: use 'layer' instead.
  // 已废弃，用layer代替。
  repeated V1LayerParameter layers = 2;
}


// NOTE
// Update the next available ID when you add a new SolverParameter field.
// 注意：当你添加一个新的SolverParameter字段时，要更新下一个可获取的ID
// SolverParameter next available ID: 41 (last added: type)
// Solver参数
message SolverParameter {
  //////////////////////////////////////////////////////////////////////////////
  // Specifying the train and test networks
  //
  // Exactly one train net must be specified using one of the following fields:
  //     train_net_param, train_net, net_param, net
  // One or more test nets may be specified using any of the following fields:
  //     test_net_param, test_net, net_param, net
  // If more than one test net field is specified (e.g., both net and
  // test_net are specified), they will be evaluated in the field order given
  // above: (1) test_net_param, (2) test_net, (3) net_param/net.
  // A test_iter must be specified for each test_net.
  // A test_level and/or a test_stage may also be specified for each test_net.
  //////////////////////////////////////////////////////////////////////////////

  // Proto filename for the train net, possibly combined with one or more test nets.
  // 训练网络的prototxt文件名，可能结合一个或多个测试网络
  optional string net = 24;
  // Inline train net param, possibly combined with one or more test nets.
  // 内联训练网络参数，可能结合一个或多个测试网络
  optional NetParameter net_param = 25;

  // 训练网络的proto文件名
  optional string train_net = 1; // Proto filename for the train net.
  // 测试网络的proto文件名
  repeated string test_net = 2; // Proto filenames for the test nets.
  // 内联训练网络参数
  optional NetParameter train_net_param = 21; // Inline train net params.
  // 内联测试网络参数
  repeated NetParameter test_net_param = 22; // Inline test net params.

  // The states for the train/test nets. Must be unspecified or specified once per net.
  // By default, all states will have solver = true;
  // train_state will have phase = TRAIN,
  // and all test_state's will have phase = TEST.
  // Other defaults are set according to the NetState defaults.
  // train/test网络的状态，必须不指定或每个网络指定一次
  // 默认情况下，所有的状态都有solver = true，train_state的phase = TRAIN，其它默认情况根据NetState默认值设定。

  // train网络的状态，必须不指定或每个网络指定一次
  optional NetState train_state = 26;
  // test网络的状态，必须不指定或每个网络指定一次
  repeated NetState test_state = 27;

  // The number of iterations for each test net.
  // 每个测试网络的迭代次数，即测试数据的迭代次数，测试数据总数=测试迭代次数*测试数据的batch_size。
  repeated int32 test_iter = 3;

  // The number of iterations between two testing phases.
  // 两次测试间隔的迭代次数，即训练数据迭代多少次进行一次测试。
  optional int32 test_interval = 4 [default = 0];
  // 测试数据的loss，默认情况下不计算
  optional bool test_compute_loss = 19 [default = false];
  // If true, run an initial test pass before the first iteration,
  // ensuring memory availability and printing the starting value of the loss.
  // 如果为true，在第一次迭代之前进行一次初始测试，从而确保内存可用性并输出初始损失值。
  optional bool test_initialization = 32 [default = true];
  // 基本学习率
  optional float base_lr = 5; // The base learning rate
  // the number of iterations between displaying info. If display = 0, no info will be displayed.
  // 执行多少次迭代显示一次信息，如果display = 0，不输出信息。
  optional int32 display = 6;
  // Display the loss averaged over the last average_loss iterations
  // 输出的平均损失是之前多少次迭代的平均损失。
  optional int32 average_loss = 33 [default = 1];
  // 训练的最大迭代次数
  optional int32 max_iter = 7; // the maximum number of iterations
  // accumulate gradients over `iter_size` x `batch_size` instances
  // 累积`iter_size` x `batch_size`个实例的梯度
  optional int32 iter_size = 36 [default = 1];

  // The learning rate decay policy. The currently implemented learning rate
  // policies are as follows:
  //    - fixed: always return base_lr.
  //    - step: return base_lr * gamma ^ (floor(iter / step))
  //    - exp: return base_lr * gamma ^ iter
  //    - inv: return base_lr * (1 + gamma * iter) ^ (- power)
  //    - multistep: similar to step but it allows non uniform steps defined by
  //      stepvalue
  //    - poly: the effective learning rate follows a polynomial decay, to be
  //      zero by the max_iter. return base_lr (1 - iter/max_iter) ^ (power)
  //    - sigmoid: the effective learning rate follows a sigmod decay
  //      return base_lr ( 1/(1 + exp(-gamma * (iter - stepsize))))
  //
  // where base_lr, max_iter, gamma, step, stepvalue and power are defined
  // in the solver parameter protocol buffer, and iter is the current iteration.

  // 学习率的变化策略
  optional string lr_policy = 8;
  // 学习率的计算参数
  optional float gamma = 9; // The parameter to compute the learning rate.
  // 学习率的计算参数
  optional float power = 10; // The parameter to compute the learning rate.
  // 动量参数
  optional float momentum = 11; // The momentum value.
  // 权重衰减，权重衰减主要影响神经网络的正则项，具体可参考Caffe文档
  optional float weight_decay = 12; // The weight decay.
  // regularization types supported: L1 and L2, controlled by weight_decay
  // 正则化类型支持L1和L2，受weight_decay控制。
  optional string regularization_type = 29 [default = "L2"];
  // the stepsize for learning rate policy "step"
  // 学习率方案为step时的参数
  optional int32 stepsize = 13;
  // the stepsize for learning rate policy "multistep"
  // 学习率方案为multistep时的参数
  repeated int32 stepvalue = 34;

  // Set clip_gradients to >= 0 to clip parameter gradients to that L2 norm,
  // whenever their actual L2 norm is larger.
  // 设置clip_gradients >= 0可以削减L2范数的梯度，当真实L2范数的梯度大于clip_gradients，将L2范数的梯度设为clip_gradients
  optional float clip_gradients = 35 [default = -1];
  // snapshot的间隔，即迭代多少次保存一次snapshot
  optional int32 snapshot = 14 [default = 0]; // The snapshot interval
  // snapshot的前缀
  optional string snapshot_prefix = 15; // The prefix for the snapshot.
  // whether to snapshot diff in the results or not. Snapshotting diff will help
  // debugging but the final protocol buffer size will be much larger.
  // 是否在结果中保存snapshot的差分，snapshot diff有助于调试，但snapshot的文件会更大。
  optional bool snapshot_diff = 16 [default = false];
  // snapshot的保存格式（hdf5,binaryproto）。
  enum SnapshotFormat {
    HDF5 = 0;
    BINARYPROTO = 1;
  }
  // snapshot默认保存为BINARYPROTO。
  optional SnapshotFormat snapshot_format = 37 [default = BINARYPROTO];
  // the mode solver will use: 0 for CPU and 1 for GPU. Use GPU in default.
  // 求解神经网络的方式，0 CPU, 1 GPU。默认使用GPU
  enum SolverMode {
    CPU = 0;
    GPU = 1;
  }
  // 求解神经网络的模式，0 CPU, 1 GPU。默认使用GPU
  optional SolverMode solver_mode = 17 [default = GPU];
  // the device_id will that be used in GPU mode. Use device_id = 0 in default.
  // device_id是GPU模式下GPU的ID。
  optional int32 device_id = 18 [default = 0];
  // If non-negative, the seed with which the Solver will initialize the Caffe
  // random number generator -- useful for reproducible results. Otherwise,
  // (and by default) initialize using a seed derived from the system clock.
  // 如果是非负值，seed用来初始化Caffe的随机数生成器，对于再见结果是很有用的，默认情况下，seed的是从系统时钟获取。
  optional int64 random_seed = 20 [default = -1];

  // type of the solver
  // 神经网络求解的类型, 默认为SGD
  optional string type = 40 [default = "SGD"];

  // numerical stability for RMSProp, AdaGrad and AdaDelta and Adam
  // RMSProp, AdaGrad, AdaDelta, Adam求解类型的参数
  optional float delta = 31 [default = 1e-8];
  // parameters for the Adam solver
  // Adam求解类型的参数
  optional float momentum2 = 39 [default = 0.999];

  // RMSProp decay value
  // MeanSquare(t) = rms_decay*MeanSquare(t-1) + (1-rms_decay)*SquareGradient(t)
  // RMSProp类型的衰减值
  optional float rms_decay = 38 [default = 0.99];

  // If true, print information about the state of the net that may help with
  // debugging learning problems.
  // 如果设为true，会输出网络的状态信息，有助于调试
  optional bool debug_info = 23 [default = false];

  // If false, don't save a snapshot after training finishes.
  // 如果设为false，不保存训练结束的snapshot。
  optional bool snapshot_after_train = 28 [default = true];

  // DEPRECATED: old solver enum types, use string instead
  // 已废弃，使用string代替
  enum SolverType {
    SGD = 0;
    NESTEROV = 1;
    ADAGRAD = 2;
    RMSPROP = 3;
    ADADELTA = 4;
    ADAM = 5;
  }
  // DEPRECATED: use type instead of solver_type
  // 已废弃：使用type代替
  optional SolverType solver_type = 30 [default = SGD];
}

// A message that stores the solver snapshots
// 保存solver snapshots
message SolverState {
  // 当前的迭代次数
  optional int32 iter = 1; // The current iteration
  // 保存学习到的网络
  optional string learned_net = 2; // The file that stores the learned net.
  // sgd的求解历史
  repeated BlobProto history = 3; // The history for sgd solvers
  // 学习的当前step
  optional int32 current_step = 4 [default = 0]; // The current step for learning rate
}

// 定义phase
enum Phase {
   TRAIN = 0;
   TEST = 1;
}

// 网络状态
message NetState {
  // 属于哪个phase
  optional Phase phase = 1 [default = TEST];
  optional int32 level = 2 [default = 0];
  repeated string stage = 3;
}

// 网络状态分类
message NetStateRule {
  // Set phase to require the NetState have a particular phase (TRAIN or TEST)
  // to meet this rule.
  // 设置phase
  optional Phase phase = 1;

  // Set the minimum and/or maximum levels in which the layer should be used.
  // Leave undefined to meet the rule regardless of level.
  // 设置layer的level
  optional int32 min_level = 2;
  optional int32 max_level = 3;

  // Customizable sets of stages to include or exclude.
  // The net must have ALL of the specified stages and NONE of the specified
  // "not_stage"s to meet the rule.
  // (Use multiple NetStateRules to specify conjunctions of stages.)
  // 可定制的stage集合
  repeated string stage = 4;
  repeated string not_stage = 5;
}

// Specifies training parameters (multipliers on global learning constants,
// and the name and other settings used for weight sharing).
// 指定训练参数及名称以及权重共享的其它设置
message ParamSpec {
  // The names of the parameter blobs -- useful for sharing parameters among
  // layers, but never required otherwise.  To share a parameter between two
  // layers, give it a (non-empty) name.
  // 两个layer之间进行参数共享的blob名字
  optional string name = 1;

  // Whether to require shared weights to have the same shape, or just the same
  // count -- defaults to STRICT if unspecified.
  // 参数共享时是否需要具有相同的shape，默认情况下需要有相同的shape
  optional DimCheckMode share_mode = 2;
  // 参数共享时的维度检查
  enum DimCheckMode {
    // STRICT (default) requires that num, channels, height, width each match.
    STRICT = 0;
    // PERMISSIVE requires only the count (num*channels*height*width) to match.
    PERMISSIVE = 1;
  }

  // The multiplier on the global learning rate for this parameter.
  // 学习率参数, learning rate = base_lr * lr_mult
  optional float lr_mult = 3 [default = 1.0];

  // The multiplier on the global weight decay for this parameter.
  // 权重衰减参数, weight = weight_decay * decay_mult
  optional float decay_mult = 4 [default = 1.0];
}

// NOTE
// Update the next available ID when you add a new LayerParameter field.
// LayerParameter next available layer-specific ID: 147 (last added: recurrent_param)
// 注意：当你添加一个新的LayerParameter字段时，要更新下一个可获取的ID
message LayerParameter {
  // layer名称
  optional string name = 1; // the layer name
  // layer类型
  optional string type = 2; // the layer type
  // layer的输入
  repeated string bottom = 3; // the name of each bottom blob
  // layer的输出
  repeated string top = 4; // the name of each top blob

  // The train / test phase for computation.
  // layer用在train/test phase
  optional Phase phase = 10;

  // The amount of weight to assign each top blob in the objective.
  // Each layer assigns a default value, usually of either 0 or 1,
  // to each top blob.
  // layer对最终的loss损失值的贡献率
  repeated float loss_weight = 5;

  // Specifies training parameters (multipliers on global learning constants,
  // and the name and other settings used for weight sharing).
  // 指定训练参数
  repeated ParamSpec param = 6;

  // The blobs containing the numeric parameters of the layer.
  // layer的blobs
  repeated BlobProto blobs = 7;

  // Specifies whether to backpropagate to each bottom. If unspecified,
  // Caffe will automatically infer whether each input needs backpropagation
  // to compute parameter gradients. If set to true for some inputs,
  // backpropagation to those inputs is forced; if set false for some inputs,
  // backpropagation to those inputs is skipped.
  //
  // The size must be either 0 or equal to the number of bottoms.
  // 指定反向传播是否传播到每一个bottom，如果不指定，caffe会自动检查推断是否每一个输入都需要反向传播来计算梯度。如果一些输入设为true,
  // 则这些layer强制进行反向传播，如果设为false，这些layer将跳过反向传播。
  repeated bool propagate_down = 11;

  // Rules controlling whether and when a layer is included in the network,
  // based on the current NetState.  You may specify a non-zero number of rules
  // to include OR exclude, but not both.  If no include or exclude rules are
  // specified, the layer is always included.  If the current NetState meets
  // ANY (i.e., one or more) of the specified rules, the layer is
  // included/excluded.
  // 控制layer included/excluded
  repeated NetStateRule include = 8;
  repeated NetStateRule exclude = 9;

  // Parameters for data pre-processing.
  // 数据预处理参数
  optional TransformationParameter transform_param = 100;

  // Parameters shared by loss layers.
  // loss layer的参数共享
  optional LossParameter loss_param = 101;

  // Layer type-specific parameters.
  //
  // Note: certain layers may have more than one computational engine
  // for their implementation. These layers include an Engine type and
  // engine parameter for selecting the implementation.
  // The default for the engine is set by the ENGINE switch at compile-time.

  // 特定layer的参数
  optional AccuracyParameter accuracy_param = 102;
  optional ArgMaxParameter argmax_param = 103;
  optional BatchNormParameter batch_norm_param = 139;
  optional BiasParameter bias_param = 141;
  optional ConcatParameter concat_param = 104;
  optional ContrastiveLossParameter contrastive_loss_param = 105;
  optional ConvolutionParameter convolution_param = 106;
  optional CropParameter crop_param = 144;
  optional DataParameter data_param = 107;
  optional DropoutParameter dropout_param = 108;
  optional DummyDataParameter dummy_data_param = 109;
  optional EltwiseParameter eltwise_param = 110;
  optional ELUParameter elu_param = 140;
  optional EmbedParameter embed_param = 137;
  optional ExpParameter exp_param = 111;
  optional FlattenParameter flatten_param = 135;
  optional HDF5DataParameter hdf5_data_param = 112;
  optional HDF5OutputParameter hdf5_output_param = 113;
  optional HingeLossParameter hinge_loss_param = 114;
  optional ImageDataParameter image_data_param = 115;
  optional InfogainLossParameter infogain_loss_param = 116;
  optional InnerProductParameter inner_product_param = 117;
  optional InputParameter input_param = 143;
  optional LogParameter log_param = 134;
  optional LRNParameter lrn_param = 118;
  optional MemoryDataParameter memory_data_param = 119;
  optional MVNParameter mvn_param = 120;
  optional ParameterParameter parameter_param = 145;
  optional PoolingParameter pooling_param = 121;
  optional PowerParameter power_param = 122;
  optional PReLUParameter prelu_param = 131;
  optional PythonParameter python_param = 130;
  optional RecurrentParameter recurrent_param = 146;
  optional ReductionParameter reduction_param = 136;
  optional ReLUParameter relu_param = 123;
  optional ReshapeParameter reshape_param = 133;
  optional ScaleParameter scale_param = 142;
  optional SigmoidParameter sigmoid_param = 124;
  optional SoftmaxParameter softmax_param = 125;
  optional SPPParameter spp_param = 132;
  optional SliceParameter slice_param = 126;
  optional TanHParameter tanh_param = 127;
  optional ThresholdParameter threshold_param = 128;
  optional TileParameter tile_param = 138;
  optional WindowDataParameter window_data_param = 129;
}

// Message that stores parameters used to apply transformation to the data layer's data
// 用来进行数据层（图像）变换的参数
message TransformationParameter {
  // For data pre-processing, we can do simple scaling and subtracting the
  // data mean, if provided. Note that the mean subtraction is always carried
  // out before scaling.
  // 像素归一化，归一化之前会减去均值
  optional float scale = 1 [default = 1];
  // Specify if we want to randomly mirror data.
  // 图像进行随机mirror操作
  optional bool mirror = 2 [default = false];
  // Specify if we would like to randomly crop an image.
  // 图像随机crop操作
  optional uint32 crop_size = 3 [default = 0];
  // mean_file and mean_value cannot be specified at the same time
  // 图像的均值文件
  optional string mean_file = 4;
  // if specified can be repeated once (would subtract it from all the channels)
  // or can be repeated the same number of times as channels
  // (would subtract them from the corresponding channel)
  // 图像的均值，手动指定，通常是三个
  repeated float mean_value = 5;
  // Force the decoded image to have 3 color channels.
  // 强制图像必须有三个颜色通道
  optional bool force_color = 6 [default = false];
  // Force the decoded image to have 1 color channels.
  // 强制图像为灰度图像
  optional bool force_gray = 7 [default = false];
}

// Message that stores parameters shared by loss layers
// loss层参数
message LossParameter {
  // If specified, ignore instances with the given label.
  // 如果指定，则label等于ignore_label的样本将不参与Loss计算，并且反向传播时梯度直接置0。
  optional int32 ignore_label = 1;
  // How to normalize the loss for loss layers that aggregate across batches,
  // spatial dimensions, or other dimensions.  Currently only implemented in
  // SoftmaxWithLoss and SigmoidCrossEntropyLoss layers.
  // 指定loss归一化的方式
  enum NormalizationMode {
    // Divide by the number of examples in the batch times spatial dimensions.
    // Outputs that receive the ignore label will NOT be ignored in computing
    // the normalization factor.
    // 所有样本都参与计算，包括ignore label
    FULL = 0;
    // Divide by the total number of output locations that do not take the
    // ignore_label.  If ignore_label is not set, this behaves like FULL.
    // 所有样本都参与计算，不包括ignore label
    VALID = 1;
    // Divide by the batch size.
    // 除以给定的batch size。
    BATCH_SIZE = 2;
    // Do not normalize the loss.
    // 不归一化loss
    NONE = 3;
  }
  // For historical reasons, the default normalization for
  // SigmoidCrossEntropyLoss is BATCH_SIZE and *not* VALID.
  // loss归一化方式
  optional NormalizationMode normalization = 3 [default = VALID];
  // Deprecated.  Ignored if normalization is specified.  If normalization
  // is not specified, then setting this to false will be equivalent to
  // normalization = BATCH_SIZE to be consistent with previous behavior.
  // 已废弃。Loss会除以参与计算的样本总数；否则Loss等于直接求和
  optional bool normalize = 2;
}

// Messages that store parameters used by individual layer types follow, in
// alphabetical order.
// accuracy层参数
message AccuracyParameter {
  // When computing accuracy, count as correct by comparing the true label to
  // the top k scoring classes.  By default, only compare to the top scoring
  // class (i.e. argmax).
  // 计算前top-k的准确率，默认计算top-1准确率
  optional uint32 top_k = 1 [default = 1];

  // The "label" axis of the prediction blob, whose argmax corresponds to the
  // predicted label -- may be negative to index from the end (e.g., -1 for the
  // last axis).  For example, if axis == 1 and the predictions are
  // (N x C x H x W), the label blob is expected to contain N*H*W ground truth
  // labels with integer values in {0, 1, ..., C-1}.
  // 指定在哪个维度上计算label
  optional int32 axis = 2 [default = 1];

  // If specified, ignore instances with the given label.
  // 如果指定，则忽略给定标签的实例
  optional int32 ignore_label = 3;
}


// 标签最大化参数，标签最大化即确定概率最大的label
message ArgMaxParameter {
  // If true produce pairs (argmax, maxval)
  // 如果为真，则生成(argmax, maxval)
  optional bool out_max_val = 1 [default = false];
  // 类别的top-k
  optional uint32 top_k = 2 [default = 1];
  // The axis along which to maximise -- may be negative to index from the
  // end (e.g., -1 for the last axis).
  // By default ArgMaxLayer maximizes over the flattened trailing dimensions
  // for each index of the first / num dimension.
  // 根据axis进行标签最大化
  optional int32 axis = 3;
}

// 参数拼接，在deconv的prototxt文件中见过
message ConcatParameter {
  // The axis along which to concatenate -- may be negative to index from the
  // end (e.g., -1 for the last axis).  Other axes must have the
  // same dimension for all the bottom blobs.
  // By default, ConcatLayer concatenates blobs along the "channels" axis (1).
  // 参数拼接时的维度，按axis进行拼接
  optional int32 axis = 2 [default = 1];

  // DEPRECATED: alias for "axis" -- does not support negative indexing.
  // 已废弃。与axis一样。
  optional uint32 concat_dim = 1 [default = 1];
}

// batch norm层的相关参数, batch norm layer通常配与scale layer一起使用，具体用法可参考Resnet结构
message BatchNormParameter {
  // If false, accumulate global mean/variance values via a moving average. If
  // true, use those accumulated values instead of computing mean/variance
  // across the batch.
  // 如果设为false，累计全部的mean/variance，如果为true，使用累计值代替batch上mean/variance的计算
  // true是使用了caffe内部的均值和方差，false是使用了每个Batch里的数据的均值和方差
  optional bool use_global_stats = 1;
  // How much does the moving average decay each iteration?
  // 每次迭代平均值衰减比例
  optional float moving_average_fraction = 2 [default = .999];
  // Small value to add to the variance estimate so that we don't divide by
  // zero.
  // variance估计时为了使除数不为0，需要加上eps
  optional float eps = 3 [default = 1e-5];
}

// bias层参数，没找到实际的应用例子
message BiasParameter {
  // The first axis of bottom[0] (the first input Blob) along which to apply
  // bottom[1] (the second input Blob).  May be negative to index from the end
  // (e.g., -1 for the last axis).
  //
  // For example, if bottom[0] is 4D with shape 100x3x40x60, the output
  // top[0] will have the same shape, and bottom[1] may have any of the
  // following shapes (for the given value of axis):
  //    (axis == 0 == -4) 100; 100x3; 100x3x40; 100x3x40x60
  //    (axis == 1 == -3)          3;     3x40;     3x40x60
  //    (axis == 2 == -2)                   40;       40x60
  //    (axis == 3 == -1)                                60
  // Furthermore, bottom[1] may have the empty shape (regardless of the value of
  // "axis") -- a scalar bias.
  optional int32 axis = 1 [default = 1];

  // (num_axes is ignored unless just one bottom is given and the bias is
  // a learned parameter of the layer.  Otherwise, num_axes is determined by the
  // number of axes by the second bottom.)
  // The number of axes of the input (bottom[0]) covered by the bias
  // parameter, or -1 to cover all axes of bottom[0] starting from `axis`.
  // Set num_axes := 0, to add a zero-axis Blob: a scalar.
  optional int32 num_axes = 2 [default = 1];

  // (filler is ignored unless just one bottom is given and the bias is
  // a learned parameter of the layer.)
  // The initialization for the learned bias parameter.
  // Default is the zero (0) initialization, resulting in the BiasLayer
  // initially performing the identity operation.
  optional FillerParameter filler = 3;
}

// 对比损失层，siamese network中使用了对比损失
message ContrastiveLossParameter {
  // margin for dissimilar pair
  // 不相似的样本对的距离保持在margin以上
  optional float margin = 1 [default = 1.0];
  // The first implementation of this cost did not exactly match the cost of
  // Hadsell et al 2006 -- using (margin - d^2) instead of (margin - d)^2.
  // legacy_version = false (the default) uses (margin - d)^2 as proposed in the
  // Hadsell paper. New models should probably use this version.
  // legacy_version = true uses (margin - d^2). This is kept to support /
  // reproduce existing models and results
  // 第一版对比损失没有完全按论文写，如果为false，则按照论文原来的公式计算
  optional bool legacy_version = 2 [default = false];
}

// 卷积层参数
message ConvolutionParameter {
  // 输出数据的个数
  optional uint32 num_output = 1; // The number of outputs for the layer
  // 是否有偏置项
  optional bool bias_term = 2 [default = true]; // whether to have bias terms

  // Pad, kernel size, and stride are all given as a single value for equal
  // dimensions in all spatial dimensions, or once per spatial dimension.
  // 卷积padding的大小
  repeated uint32 pad = 3; // The padding size; defaults to 0
  // 卷积核的大小
  repeated uint32 kernel_size = 4; // The kernel size
  // 卷积的步长
  repeated uint32 stride = 6; // The stride; defaults to 1
  // Factor used to dilate the kernel, (implicitly) zero-filling the resulting
  // holes. (Kernel dilation is sometimes referred to by its use in the
  // algorithme à trous from Holschneider et al. 1987.)
  // 卷积膨胀，在卷积的时候可以skip一定长度的像素
  repeated uint32 dilation = 18; // The dilation; defaults to 1

  // For 2D convolution only, the *_h and *_w versions may also be used to
  // specify both spatial dimensions.
  // padding, kernel, stride的宽度和高度
  optional uint32 pad_h = 9 [default = 0]; // The padding height (2D only)
  optional uint32 pad_w = 10 [default = 0]; // The padding width (2D only)
  optional uint32 kernel_h = 11; // The kernel height (2D only)
  optional uint32 kernel_w = 12; // The kernel width (2D only)
  optional uint32 stride_h = 13; // The stride height (2D only)
  optional uint32 stride_w = 14; // The stride width (2D only)

  // 来自于AlexNet论文
  optional uint32 group = 5 [default = 1]; // The group size for group conv

  // 权重初始化
  optional FillerParameter weight_filler = 7; // The filler for the weight
  // 偏置初始化
  optional FillerParameter bias_filler = 8; // The filler for the bias
  enum Engine {
    DEFAULT = 0;
    CAFFE = 1;
    CUDNN = 2;
  }
  // 卷积的方式的选择，default是正常的卷积，caffe是矩阵乘法的卷积，cudnn是cuda库流并行式的卷积
  optional Engine engine = 15 [default = DEFAULT];

  // The axis to interpret as "channels" when performing convolution.
  // Preceding dimensions are treated as independent inputs;
  // succeeding dimensions are treated as "spatial".
  // With (N, C, H, W) inputs, and axis == 1 (the default), we perform
  // N independent 2D convolutions, sliding C-channel (or (C/g)-channels, for
  // groups g>1) filters across the spatial axes (H, W) of the input.
  // With (N, C, D, H, W) inputs, and axis == 1, we perform
  // N independent 3D convolutions, sliding (C/g)-channels
  // filters across the spatial axes (D, H, W) of the input.
  // 通道channel所在的维度
  optional int32 axis = 16 [default = 1];

  // Whether to force use of the general ND convolution, even if a specific
  // implementation for blobs of the appropriate number of spatial dimensions
  // is available. (Currently, there is only a 2D-specific convolution
  // implementation; for input blobs with num_axes != 2, this option is
  // ignored and the ND implementation will be used.)
  // 如果输入数据维度等于2，则执行通用的ND卷积，否则正常执行卷积
  optional bool force_nd_im2col = 17 [default = false];
}

// 图像裁剪参数
message CropParameter {
  // To crop, elements of the first bottom are selected to fit the dimensions
  // of the second, reference bottom. The crop is configured by
  // - the crop `axis` to pick the dimensions for cropping
  // - the crop `offset` to set the shift for all/each dimension
  // to align the cropped bottom with the reference bottom.
  // All dimensions up to but excluding `axis` are preserved, while
  // the dimensions including and trailing `axis` are cropped.
  // If only one `offset` is set, then all dimensions are offset by this amount.
  // Otherwise, the number of offsets must equal the number of cropped axes to
  // shift the crop in each dimension accordingly.
  // Note: standard dimensions are N,C,H,W so the default is a spatial crop,
  // and `axis` may be negative to index from the end (e.g., -1 for the last
  // axis).
  // axis是在哪个维度上进行裁剪，会裁剪轴2及之后的所有轴
  optional int32 axis = 1 [default = 2];
  // offset设置是每个维度进行裁剪时的偏移量
  repeated uint32 offset = 2;
}

// 数据层参数
message DataParameter {
  enum DB {
    LEVELDB = 0;
    LMDB = 1;
  }
  // Specify the data source.
  // 设定数据源路径
  optional string source = 1;
  // Specify the batch size.
  // 指定一次处理的图片数量
  optional uint32 batch_size = 4;
  // The rand_skip variable is for the data layer to skip a few data points
  // to avoid all asynchronous sgd clients to start at the same point. The skip
  // point would be set as rand_skip * rand(0,1). Note that rand_skip should not
  // be larger than the number of keys in the database.
  // DEPRECATED. Each solver accesses a different subset of the database.
  // rand_skip跳过指定的数据点，避免异步的sgd从同一个数据点开始
  optional uint32 rand_skip = 7 [default = 0];
  // 使用的数据库类型，LMDB or LEVELDB
  optional DB backend = 8 [default = LEVELDB];
  // DEPRECATED. See TransformationParameter. For data pre-processing, we can do
  // simple scaling and subtracting the data mean, if provided. Note that the
  // mean subtraction is always carried out before scaling.
  // 已废弃。图像归一化，在TransformationParameter中。
  optional float scale = 2 [default = 1];
  // 已废弃。均值文件，在TransformationParameter中。
  optional string mean_file = 3;
  // DEPRECATED. See TransformationParameter. Specify if we would like to randomly
  // crop an image.
  // 已废弃。图像裁剪，在TransformationParameter中。
  optional uint32 crop_size = 5 [default = 0];
  // DEPRECATED. See TransformationParameter. Specify if we want to randomly mirror
  // data.
  // 已废弃。图像翻转，在TransformationParameter中。
  optional bool mirror = 6 [default = false];
  // Force the encoded image to have 3 color channels
  // 强制图像数据有三个颜色通道
  optional bool force_encoded_color = 9 [default = false];
  // Prefetch queue (Number of batches to prefetch to host memory, increase if
  // data access bandwidth varies).
  // 预先拉取batch的数目
  optional uint32 prefetch = 10 [default = 4];
}

// dropout层参数
message DropoutParameter {
  // 为了避免过拟合，参数随机失活的比例
  optional float dropout_ratio = 1 [default = 0.5]; // dropout ratio
}


// DummyDataLayer fills any number of arbitrarily shaped blobs with random
// (or constant) data generated by "Fillers" (see "message FillerParameter").
// DummyData层的参数
message DummyDataParameter {
  // This layer produces N >= 1 top blobs.  DummyDataParameter must specify 1 or N
  // shape fields, and 0, 1 or N data_fillers.
  // If 0 data_fillers are specified, ConstantFiller with a value of 0 is used.
  // If 1 data_filler is specified, it is applied to all top blobs.  If N are
  // specified, the ith is applied to the ith top blob.
  // blob数据的生成方式
  repeated FillerParameter data_filler = 1;
  // 数据的维度
  repeated BlobShape shape = 6;

  // 4D dimensions -- deprecated.  Use "shape" instead.
  // 已废弃。使用shape代替。
  repeated uint32 num = 2;
  repeated uint32 channels = 3;
  repeated uint32 height = 4;
  repeated uint32 width = 5;
}

//Eltwise层的参数
message EltwiseParameter {
  // 操作的类型
  enum EltwiseOp {
    PROD = 0;
    SUM = 1;
    MAX = 2;
  }
  // 数据操作分三种：点乘，相加，取最大值
  optional EltwiseOp operation = 1 [default = SUM]; // element-wise operation
  // SUM操作时各个blob对应的系数
  repeated float coeff = 2; // blob-wise coefficient for SUM operation

  // Whether to use an asymptotically slower (for >2 inputs) but stabler method
  // of computing the gradient for the PROD operation. (No effect for SUM op.)
  // 在进行PROD操作，即乘法时是否使用异步操作来计算梯度，更慢但更稳定。
  optional bool stable_prod_grad = 3 [default = true];
}

// Message that stores parameters used by ELULayer
// ELU层的参数，具体看论文
message ELUParameter {
  // Described in:
  // Clevert, D.-A., Unterthiner, T., & Hochreiter, S. (2015). Fast and Accurate
  // Deep Network Learning by Exponential Linear Units (ELUs). arXiv
  optional float alpha = 1 [default = 1];
}

// Message that stores parameters used by EmbedLayer
// Embed层的参数，主要用于LSTM等翻译网络
message EmbedParameter {
  // Embed层的输出
  optional uint32 num_output = 1; // The number of outputs for the layer
  // The input is given as integers to be interpreted as one-hot
  // vector indices with dimension num_input.  Hence num_input should be
  // 1 greater than the maximum possible input value.
  // Embed层的输入
  optional uint32 input_dim = 2;
  // 是否使用偏置项
  optional bool bias_term = 3 [default = true]; // Whether to use a bias term
  // 权重生成
  optional FillerParameter weight_filler = 4; // The filler for the weight
  // 偏置生成
  optional FillerParameter bias_filler = 5; // The filler for the bias

}

// Message that stores parameters used by ExpLayer
// Exp层的参数，即指数层参数
message ExpParameter {
  // ExpLayer computes outputs y = base ^ (shift + scale * x), for base > 0.
  // Or if base is set to the default (-1), base is set to e,
  // so y = exp(shift + scale * x).
  // 指数层的计算是y = base ^ (shift + scale * x)，下面分别是公式中的三个参数
  optional float base = 1 [default = -1.0];
  optional float scale = 2 [default = 1.0];
  optional float shift = 3 [default = 0.0];
}


// Message that stores parameters used by FlattenLayer
// Flatten层的参数，主要是按某个轴展开（平铺），mnist demo的mnist_autoencode就使用了Flatten层
message FlattenParameter {
  // The first axis to flatten: all preceding axes are retained in the output.
  // May be negative to index from the end (e.g., -1 for the last axis).
  // 从哪一层开始展开
  optional int32 axis = 1 [default = 1];

  // The last axis to flatten: all following axes are retained in the output.
  // May be negative to index from the end (e.g., the default -1 for the last
  // axis).
  // 展开到哪一层结束
  optional int32 end_axis = 2 [default = -1];
}

// Message that stores parameters used by HDF5DataLayer
// HDF5数据层的参数
message HDF5DataParameter {
  // Specify the data source.
  // HDF5层输入数据的数据源
  optional string source = 1;
  // Specify the batch size.
  // 训练的batch_size
  optional uint32 batch_size = 2;

  // Specify whether to shuffle the data.
  // If shuffle == true, the ordering of the HDF5 files is shuffled,
  // and the ordering of data within any given HDF5 file is shuffled,
  // but data between different files are not interleaved; all of a file's
  // data are output (in a random order) before moving onto another file.
  // 是否对HDF5的输入数据进行shuffle
  optional bool shuffle = 3 [default = false];
}

// HDF5输出层参数
message HDF5OutputParameter {
  // 输出的HDF5文件的文件名
  optional string file_name = 1;
}

// HingeLoss层参数
message HingeLossParameter {
  enum Norm {
    L1 = 1;
    L2 = 2;
  }
  // Specify the Norm to use L1 or L2
  // 指定HingeLoss的类型
  optional Norm norm = 1 [default = L1];
}


// ImageData层参数，网络中直接输入原图
message ImageDataParameter {
  // Specify the data source.
  // 描述图像路径及标签的文件
  optional string source = 1;
  // Specify the batch size.
  // 训练的batch size
  optional uint32 batch_size = 4 [default = 1];
  // The rand_skip variable is for the data layer to skip a few data points
  // to avoid all asynchronous sgd clients to start at the same point. The skip
  // point would be set as rand_skip * rand(0,1). Note that rand_skip should not
  // be larger than the number of keys in the database.
  // rand_skip跳过指定的数据点，避免异步的sgd从同一个数据点开始，与Data层中是一样的
  optional uint32 rand_skip = 7 [default = 0];
  // Whether or not ImageLayer should shuffle the list of files at every epoch.
  // 是否对图像顺序进行shuffle
  optional bool shuffle = 8 [default = false];
  // It will also resize images if new_height or new_width are not zero.
  // 图像resize的高度
  optional uint32 new_height = 9 [default = 0];
  // 图像resize的宽度
  optional uint32 new_width = 10 [default = 0];
  // Specify if the images are color or gray
  // 指定图像彩色图像还是灰度图像，默认彩色
  optional bool is_color = 11 [default = true];
  // DEPRECATED. See TransformationParameter. For data pre-processing, we can do
  // simple scaling and subtracting the data mean, if provided. Note that the
  // mean subtraction is always carried out before scaling.
  // 已废弃。参考TransformationParameter中的scale
  optional float scale = 2 [default = 1];
  // 指定均值文件
  optional string mean_file = 3;
  // DEPRECATED. See TransformationParameter. Specify if we would like to randomly
  // crop an image.
  // 已废弃。参考TransformationParameter中的crop_size
  optional uint32 crop_size = 5 [default = 0];
  // DEPRECATED. See TransformationParameter. Specify if we want to randomly mirror
  // data.
  // 已废弃，参考TransformationParameter的mirror。
  optional bool mirror = 6 [default = false];
  // 不太清楚root_folder具体是什么
  optional string root_folder = 12 [default = ""];
}

// 信息增益损失层参数
message InfogainLossParameter {
  // Specify the infogain matrix source.
  // 指定存储信息增益矩阵的源文件
  optional string source = 1;
}

// InnerProduct层的参数
message InnerProductParameter {
  // InnerProduct层的输出
  optional uint32 num_output = 1; // The number of outputs for the layer
  // 是否有偏置项
  optional bool bias_term = 2 [default = true]; // whether to have bias terms
  // 权重初始化，随机生成
  optional FillerParameter weight_filler = 3; // The filler for the weight
  // 偏置初始化，随机生成
  optional FillerParameter bias_filler = 4; // The filler for the bias

  // The first axis to be lumped into a single inner product computation;
  // all preceding axes are retained in the output.
  // May be negative to index from the end (e.g., -1 for the last axis).
  // 从某一维度开始进行内积计算，前面的维度保留
  optional int32 axis = 5 [default = 1];
  // Specify whether to transpose the weight matrix or not.
  // If transpose == true, any operations will be performed on the transpose
  // of the weight matrix. The weight matrix itself is not going to be transposed
  // but rather the transfer flag of operations will be toggled accordingly.
  // 是否对权重矩阵进行转置
  optional bool transpose = 6 [default = false];
}

// Input参数，caffe网络部署时会用到
message InputParameter {
  // This layer produces N >= 1 top blob(s) to be assigned manually.
  // Define N shapes to set a shape for each top.
  // Define 1 shape to set the same shape for every top.
  // Define no shape to defer to reshaping manually.
  // 输入数据的shape
  repeated BlobShape shape = 1;
}

// Message that stores parameters used by LogLayer
// Log层参数，对数据进行Log运算
message LogParameter {
  // LogLayer computes outputs y = log_base(shift + scale * x), for base > 0.
  // Or if base is set to the default (-1), base is set to e,
  // so y = ln(shift + scale * x) = log_e(shift + scale * x)
  // Log层计算公式为y = log_base(shift + scale * x)，下面分别是公式中的三个参数
  optional float base = 1 [default = -1.0];
  optional float scale = 2 [default = 1.0];
  optional float shift = 3 [default = 0.0];
}

// Message that stores parameters used by LRNLayer
// LRN层的参数，局部归一化，AlexNet中的LRN
message LRNParameter {
  // 如果是跨通道LRN，则表示求和的通道数；如果是在通道内LRN，则表示求和的正方形区域长度。
  optional uint32 local_size = 1 [default = 5];
  // 归一化公式中的参数
  optional float alpha = 2 [default = 1.];
  optional float beta = 3 [default = 0.75];
  enum NormRegion {
    ACROSS_CHANNELS = 0;
    WITHIN_CHANNEL = 1;
  }
  // 归一化的区域，分为通道内和跨通道两种
  optional NormRegion norm_region = 4 [default = ACROSS_CHANNELS];
  optional float k = 5 [default = 1.];
  enum Engine {
    DEFAULT = 0;
    CAFFE = 1;
    CUDNN = 2;
  }
  // 与前面的engine是一样的
  optional Engine engine = 6 [default = DEFAULT];
}

// 内存数据层参数
message MemoryDataParameter {
  // 训练的batch_size
  optional uint32 batch_size = 1;
  // 图像通道数
  optional uint32 channels = 2;
  // 图像高度
  optional uint32 height = 3;
  // 图像宽度
  optional uint32 width = 4;
}

// mean-variance normalization层参数
message MVNParameter {
  // This parameter can be set to false to normalize mean only
  // 是否对方差进行归一化
  optional bool normalize_variance = 1 [default = true];

  // This parameter can be set to true to perform DNN-like MVN
  // 是否进行跨通道的MVN
  optional bool across_channels = 2 [default = false];

  // Epsilon for not dividing by zero while normalizing variance
  // 避免除数为0，与前面的一样
  optional float eps = 3 [default = 1e-9];
}

// 参数层参数
message ParameterParameter {
  // 用户自己定义的shape
  optional BlobShape shape = 1;
}

// 池化层参数
message PoolingParameter {
  enum PoolMethod {
    MAX = 0;
    AVE = 1;
    STOCHASTIC = 2;
  }
  // 池化的方式
  optional PoolMethod pool = 1 [default = MAX]; // The pooling method
  // Pad, kernel size, and stride are all given as a single value for equal
  // dimensions in height and width or as Y, X pairs.
  // padding的大小
  optional uint32 pad = 4 [default = 0]; // The padding size (equal in Y, X)
  // padding的高度
  optional uint32 pad_h = 9 [default = 0]; // The padding height
  // padding的宽度
  optional uint32 pad_w = 10 [default = 0]; // The padding width
  // 池化的核大小
  optional uint32 kernel_size = 2; // The kernel size (square)
  // 核高度
  optional uint32 kernel_h = 5; // The kernel height
  // 核宽度
  optional uint32 kernel_w = 6; // The kernel width
  // 池化的步长
  optional uint32 stride = 3 [default = 1]; // The stride (equal in Y, X)
  // 步长的高度
  optional uint32 stride_h = 7; // The stride height
  // 步长的宽度
  optional uint32 stride_w = 8; // The stride width
  enum Engine {
    DEFAULT = 0;
    CAFFE = 1;
    CUDNN = 2;
  }
  // 执行池化操作的类型，与前面的一样
  optional Engine engine = 11 [default = DEFAULT];
  // If global_pooling then it will pool over the size of the bottom by doing
  // kernel_h = bottom->height and kernel_w = bottom->width
  // global_pooling是对多个通道进行pooling，例如从三通道pooling为单通道
  optional bool global_pooling = 12 [default = false];
}

// Power层参数
message PowerParameter {
  // PowerLayer computes outputs y = (shift + scale * x) ^ power.
  // Power的计算公式为y = (shift + scale * x) ^ power，下面是公式中的参数
  optional float power = 1 [default = 1.0];
  optional float scale = 2 [default = 1.0];
  optional float shift = 3 [default = 0.0];
}

// python layer参数，在faster rcnn中有应用
message PythonParameter {
  // python模块名称
  optional string module = 1;
  // python模块中层的名字，即类名
  optional string layer = 2;
  // This value is set to the attribute `param_str` of the `PythonLayer` object
  // in Python before calling the `setup()` method. This could be a number,
  // string, dictionary in Python dict format, JSON, etc. You may parse this
  // string in `setup` method and use it in `forward` and `backward`.
  // 可以用来设置参数，key-value形式，可以参考faster rcnn中模型的train.prototxt
  optional string param_str = 3 [default = ''];
  // Whether this PythonLayer is shared among worker solvers during data parallelism.
  // If true, each worker solver sequentially run forward from this layer.
  // This value should be set true if you are using it as a data layer.
  // 是否需要在并行时共享layer
  optional bool share_in_parallel = 4 [default = false];
}


// Message that stores parameters used by RecurrentLayer
// Recurrent层参数
message RecurrentParameter {
  // The dimension of the output (and usually hidden state) representation --
  // must be explicitly set to non-zero.
  // Recurrent层的输出——必须非零
  optional uint32 num_output = 1 [default = 0];
  // 权重初始化，随机生成初始化
  optional FillerParameter weight_filler = 2; // The filler for the weight
  // 偏置初始化，随机生成
  optional FillerParameter bias_filler = 3; // The filler for the bias

  // Whether to enable displaying debug_info in the unrolled recurrent net.
  // 是否输出调试信息
  optional bool debug_info = 4 [default = false];

  // Whether to add as additional inputs (bottoms) the initial hidden state
  // blobs, and add as additional outputs (tops) the final timestep hidden state
  // blobs.  The number of additional bottom/top blobs required depends on the
  // recurrent architecture -- e.g., 1 for RNNs, 2 for LSTMs.
  // 是否添加额外的输入
  optional bool expose_hidden = 5 [default = false];
}

// Message that stores parameters used by ReductionLayer
// Reduction层参数
message ReductionParameter {
  enum ReductionOp {
    SUM = 1;
    ASUM = 2;
    SUMSQ = 3;
    MEAN = 4;
  }
  // 通过reduction操作来将数据减少到一维，可以通过上面的四种方式
  optional ReductionOp operation = 1 [default = SUM]; // reduction operation

  // The first axis to reduce to a scalar -- may be negative to index from the
  // end (e.g., -1 for the last axis).
  // (Currently, only reduction along ALL "tail" axes is supported; reduction
  // of axis M through N, where N < num_axes - 1, is unsupported.)
  // Suppose we have an n-axis bottom Blob with shape:
  //     (d0, d1, d2, ..., d(m-1), dm, d(m+1), ..., d(n-1)).
  // If axis == m, the output Blob will have shape
  //     (d0, d1, d2, ..., d(m-1)),
  // and the ReductionOp operation is performed (d0 * d1 * d2 * ... * d(m-1))
  // times, each including (dm * d(m+1) * ... * d(n-1)) individual data.
  // If axis == 0 (the default), the output Blob always has the empty shape
  // (count 1), performing reduction across the entire input --
  // often useful for creating new loss functions.
  // 在哪个轴上执行reduction操作
  optional int32 axis = 2 [default = 0];
  // 输出系数
  optional float coeff = 3 [default = 1.0]; // coefficient for output
}

// Message that stores parameters used by ReLULayer
// ReLU层参数
message ReLUParameter {
  // Allow non-zero slope for negative inputs to speed up optimization
  // Described in:
  // Maas, A. L., Hannun, A. Y., & Ng, A. Y. (2013). Rectifier nonlinearities
  // improve neural network acoustic models. In ICML Workshop on Deep Learning
  // for Audio, Speech, and Language Processing.
  // ReLUU操作的阈值
  optional float negative_slope = 1 [default = 0];
  enum Engine {
    DEFAULT = 0;
    CAFFE = 1;
    CUDNN = 2;
  }
  // 执行ReLU操作的类型，与前面的一样
  optional Engine engine = 2 [default = DEFAULT];
}

// Reshape层参数，与numpy中的Reshape作用是一样的
message ReshapeParameter {
  // Specify the output dimensions. If some of the dimensions are set to 0,
  // the corresponding dimension from the bottom layer is used (unchanged).
  // Exactly one dimension may be set to -1, in which case its value is
  // inferred from the count of the bottom blob and the remaining dimensions.
  // For example, suppose we want to reshape a 2D blob "input" with shape 2 x 8:
  //
  //   layer {
  //     type: "Reshape" bottom: "input" top: "output"
  //     reshape_param { ... }
  //   }
  //
  // If "input" is 2D with shape 2 x 8, then the following reshape_param
  // specifications are all equivalent, producing a 3D blob "output" with shape
  // 2 x 2 x 4:
  //
  //   reshape_param { shape { dim:  2  dim: 2  dim:  4 } }
  //   reshape_param { shape { dim:  0  dim: 2  dim:  4 } }
  //   reshape_param { shape { dim:  0  dim: 2  dim: -1 } }
  //   reshape_param { shape { dim:  0  dim:-1  dim:  4 } }
  // reshape之后输出的维度
  optional BlobShape shape = 1;

  // axis and num_axes control the portion of the bottom blob's shape that are
  // replaced by (included in) the reshape. By default (axis == 0 and
  // num_axes == -1), the entire bottom blob shape is included in the reshape,
  // and hence the shape field must specify the entire output shape.
  //
  // axis may be non-zero to retain some portion of the beginning of the input
  // shape (and may be negative to index from the end; e.g., -1 to begin the
  // reshape after the last axis, including nothing in the reshape,
  // -2 to include only the last axis, etc.).
  //
  // For example, suppose "input" is a 2D blob with shape 2 x 8.
  // Then the following ReshapeLayer specifications are all equivalent,
  // producing a blob "output" with shape 2 x 2 x 4:
  //
  //   reshape_param { shape { dim: 2  dim: 2  dim: 4 } }
  //   reshape_param { shape { dim: 2  dim: 4 } axis:  1 }
  //   reshape_param { shape { dim: 2  dim: 4 } axis: -3 }
  //
  // num_axes specifies the extent of the reshape.
  // If num_axes >= 0 (and axis >= 0), the reshape will be performed only on
  // input axes in the range [axis, axis+num_axes].
  // num_axes may also be -1, the default, to include all remaining axes
  // (starting from axis).
  //
  // For example, suppose "input" is a 2D blob with shape 2 x 8.
  // Then the following ReshapeLayer specifications are equivalent,
  // producing a blob "output" with shape 1 x 2 x 8.
  //
  //   reshape_param { shape { dim:  1  dim: 2  dim:  8 } }
  //   reshape_param { shape { dim:  1  dim: 2  }  num_axes: 1 }
  //   reshape_param { shape { dim:  1  }  num_axes: 0 }
  //
  // On the other hand, these would produce output blob shape 2 x 1 x 8:
  //
  //   reshape_param { shape { dim: 2  dim: 1  dim: 8  }  }
  //   reshape_param { shape { dim: 1 }  axis: 1  num_axes: 0 }

  optional int32 axis = 2 [default = 0];
  optional int32 num_axes = 3 [default = -1];
}

// Scale层参数，与batch norm layer配合使用，可参考Resnet结构
message ScaleParameter {
  // The first axis of bottom[0] (the first input Blob) along which to apply
  // bottom[1] (the second input Blob).  May be negative to index from the end
  // (e.g., -1 for the last axis).
  //
  // For example, if bottom[0] is 4D with shape 100x3x40x60, the output
  // top[0] will have the same shape, and bottom[1] may have any of the
  // following shapes (for the given value of axis):
  //    (axis == 0 == -4) 100; 100x3; 100x3x40; 100x3x40x60
  //    (axis == 1 == -3)          3;     3x40;     3x40x60
  //    (axis == 2 == -2)                   40;       40x60
  //    (axis == 3 == -1)                                60
  // Furthermore, bottom[1] may have the empty shape (regardless of the value of
  // "axis") -- a scalar multiplier.
  optional int32 axis = 1 [default = 1];

  // (num_axes is ignored unless just one bottom is given and the scale is
  // a learned parameter of the layer.  Otherwise, num_axes is determined by the
  // number of axes by the second bottom.)
  // The number of axes of the input (bottom[0]) covered by the scale
  // parameter, or -1 to cover all axes of bottom[0] starting from `axis`.
  // Set num_axes := 0, to multiply with a zero-axis Blob: a scalar.
  optional int32 num_axes = 2 [default = 1];

  // (filler is ignored unless just one bottom is given and the scale is
  // a learned parameter of the layer.)
  // The initialization for the learned scale parameter.
  // Default is the unit (1) initialization, resulting in the ScaleLayer
  // initially performing the identity operation.
  optional FillerParameter filler = 3;

  // Whether to also learn a bias (equivalent to a ScaleLayer+BiasLayer, but
  // may be more efficient).  Initialized with bias_filler (defaults to 0).
  // 是否使用偏置项
  optional bool bias_term = 4 [default = false];
  // 偏置项初始化
  optional FillerParameter bias_filler = 5;
}

// Sigmoid层参数
message SigmoidParameter {
  enum Engine {
    DEFAULT = 0;
    CAFFE = 1;
    CUDNN = 2;
  }
  // 使用哪种sigmoid实现
  optional Engine engine = 1 [default = DEFAULT];
}

// Slice层参数
message SliceParameter {
  // The axis along which to slice -- may be negative to index from the end
  // (e.g., -1 for the last axis).
  // By default, SliceLayer concatenates blobs along the "channels" axis (1).
  // 在哪个维度上进行拆分
  optional int32 axis = 3 [default = 1];
  // 指定拆分点
  repeated uint32 slice_point = 2;

  // DEPRECATED: alias for "axis" -- does not support negative indexing.
  // 已废弃。
  optional uint32 slice_dim = 1 [default = 1];
}

// Message that stores parameters used by SoftmaxLayer, SoftmaxWithLossLayer
// Softmax层参数
message SoftmaxParameter {
  enum Engine {
    DEFAULT = 0;
    CAFFE = 1;
    CUDNN = 2;
  }
  // 使用哪种softmax实现
  optional Engine engine = 1 [default = DEFAULT];

  // The axis along which to perform the softmax -- may be negative to index
  // from the end (e.g., -1 for the last axis).
  // Any other axes will be evaluated as independent softmaxes.
  // 在哪个维度上进行softmax
  optional int32 axis = 2 [default = 1];
}

// TanH层参数
message TanHParameter {
  enum Engine {
    DEFAULT = 0;
    CAFFE = 1;
    CUDNN = 2;
  }
  // 执行tanh激活函数的类型
  optional Engine engine = 1 [default = DEFAULT];
}

// Message that stores parameters used by TileLayer
// Tile层参数，扩大某一维度
message TileParameter {
  // The index of the axis to tile.
  // 扩大哪个维度
  optional int32 axis = 1 [default = 1];

  // The number of copies (tiles) of the blob to output.
  // 创建多少个副本
  optional int32 tiles = 2;
}

// Message that stores parameters used by ThresholdLayer
// Threshold层参数，主要用来测试输入是否超过阈值
message ThresholdParameter {
  // 设置阈值
  optional float threshold = 1 [default = 0]; // Strictly positive values
}

// WindowData层参数
message WindowDataParameter {
  // Specify the data source.
  // 指定数据源
  optional string source = 1;
  // For data pre-processing, we can do simple scaling and subtracting the
  // data mean, if provided. Note that the mean subtraction is always carried
  // out before scaling.
  // 是否归一化
  optional float scale = 2 [default = 1];
  // 图像均值文件
  optional string mean_file = 3;
  // Specify the batch size.
  // 训练的batch_size
  optional uint32 batch_size = 4;
  // Specify if we would like to randomly crop an image.
  // 是否随机crop
  optional uint32 crop_size = 5 [default = 0];
  // Specify if we want to randomly mirror data.
  // 是否随机mirror
  optional bool mirror = 6 [default = false];
  // Foreground (object) overlap threshold
  // 前景重叠阈值
  optional float fg_threshold = 7 [default = 0.5];
  // Background (non-object) overlap threshold
  // 背景重叠阈值
  optional float bg_threshold = 8 [default = 0.5];
  // Fraction of batch that should be foreground objects
  // 前景比例
  optional float fg_fraction = 9 [default = 0.25];
  // Amount of contextual padding to add around a window
  // (used only by the window_data_layer)
  // 是否padding
  optional uint32 context_pad = 10 [default = 0];
  // Mode for cropping out a detection window
  // warp: cropped window is warped to a fixed size and aspect ratio
  // square: the tightest square around the window is cropped
  // crop的方式
  optional string crop_mode = 11 [default = "warp"];
  // cache_images: will load all images in memory for faster access
  // 是否缓存图像，即将图像都转入内存
  optional bool cache_images = 12 [default = false];
  // append root_folder to locate images
  // 图像文件的根目录
  optional string root_folder = 13 [default = ""];
}

// SPP层参数，SPP是spatial pyramid pooling，空间金字塔池化，具体可参考何凯明论文Spatial Pyramid Pooling in Deep Convolutional Networks for Visual Recognition
message SPPParameter {
  enum PoolMethod {
    MAX = 0;
    AVE = 1;
    STOCHASTIC = 2;
  }
  // 空间金字塔高度
  optional uint32 pyramid_height = 1;
  // 池化方法
  optional PoolMethod pool = 2 [default = MAX]; // The pooling method
  enum Engine {
    DEFAULT = 0;
    CAFFE = 1;
    CUDNN = 2;
  }
  // 执行SPP的方式
  optional Engine engine = 6 [default = DEFAULT];
}

// DEPRECATED: use LayerParameter.
// 已废弃，使用LayerParameter。
message V1LayerParameter {
  repeated string bottom = 2;
  repeated string top = 3;
  optional string name = 4;
  repeated NetStateRule include = 32;
  repeated NetStateRule exclude = 33;
  enum LayerType {
    NONE = 0;
    ABSVAL = 35;
    ACCURACY = 1;
    ARGMAX = 30;
    BNLL = 2;
    CONCAT = 3;
    CONTRASTIVE_LOSS = 37;
    CONVOLUTION = 4;
    DATA = 5;
    DECONVOLUTION = 39;
    DROPOUT = 6;
    DUMMY_DATA = 32;
    EUCLIDEAN_LOSS = 7;
    ELTWISE = 25;
    EXP = 38;
    FLATTEN = 8;
    HDF5_DATA = 9;
    HDF5_OUTPUT = 10;
    HINGE_LOSS = 28;
    IM2COL = 11;
    IMAGE_DATA = 12;
    INFOGAIN_LOSS = 13;
    INNER_PRODUCT = 14;
    LRN = 15;
    MEMORY_DATA = 29;
    MULTINOMIAL_LOGISTIC_LOSS = 16;
    MVN = 34;
    POOLING = 17;
    POWER = 26;
    RELU = 18;
    SIGMOID = 19;
    SIGMOID_CROSS_ENTROPY_LOSS = 27;
    SILENCE = 36;
    SOFTMAX = 20;
    SOFTMAX_LOSS = 21;
    SPLIT = 22;
    SLICE = 33;
    TANH = 23;
    WINDOW_DATA = 24;
    THRESHOLD = 31;
  }
  optional LayerType type = 5;
  repeated BlobProto blobs = 6;
  repeated string param = 1001;
  repeated DimCheckMode blob_share_mode = 1002;
  enum DimCheckMode {
    STRICT = 0;
    PERMISSIVE = 1;
  }
  repeated float blobs_lr = 7;
  repeated float weight_decay = 8;
  repeated float loss_weight = 35;
  optional AccuracyParameter accuracy_param = 27;
  optional ArgMaxParameter argmax_param = 23;
  optional ConcatParameter concat_param = 9;
  optional ContrastiveLossParameter contrastive_loss_param = 40;
  optional ConvolutionParameter convolution_param = 10;
  optional DataParameter data_param = 11;
  optional DropoutParameter dropout_param = 12;
  optional DummyDataParameter dummy_data_param = 26;
  optional EltwiseParameter eltwise_param = 24;
  optional ExpParameter exp_param = 41;
  optional HDF5DataParameter hdf5_data_param = 13;
  optional HDF5OutputParameter hdf5_output_param = 14;
  optional HingeLossParameter hinge_loss_param = 29;
  optional ImageDataParameter image_data_param = 15;
  optional InfogainLossParameter infogain_loss_param = 16;
  optional InnerProductParameter inner_product_param = 17;
  optional LRNParameter lrn_param = 18;
  optional MemoryDataParameter memory_data_param = 22;
  optional MVNParameter mvn_param = 34;
  optional PoolingParameter pooling_param = 19;
  optional PowerParameter power_param = 21;
  optional ReLUParameter relu_param = 30;
  optional SigmoidParameter sigmoid_param = 38;
  optional SoftmaxParameter softmax_param = 39;
  optional SliceParameter slice_param = 31;
  optional TanHParameter tanh_param = 37;
  optional ThresholdParameter threshold_param = 25;
  optional WindowDataParameter window_data_param = 20;
  optional TransformationParameter transform_param = 36;
  optional LossParameter loss_param = 42;
  optional V0LayerParameter layer = 1;
}

// DEPRECATED: V0LayerParameter is the old way of specifying layer parameters
// in Caffe.  We keep this message type around for legacy support.
// 已废弃。
message V0LayerParameter {
  optional string name = 1; // the layer name
  optional string type = 2; // the string to specify the layer type

  // Parameters to specify layers with inner products.
  optional uint32 num_output = 3; // The number of outputs for the layer
  optional bool biasterm = 4 [default = true]; // whether to have bias terms
  optional FillerParameter weight_filler = 5; // The filler for the weight
  optional FillerParameter bias_filler = 6; // The filler for the bias

  optional uint32 pad = 7 [default = 0]; // The padding size
  optional uint32 kernelsize = 8; // The kernel size
  optional uint32 group = 9 [default = 1]; // The group size for group conv
  optional uint32 stride = 10 [default = 1]; // The stride
  enum PoolMethod {
    MAX = 0;
    AVE = 1;
    STOCHASTIC = 2;
  }
  optional PoolMethod pool = 11 [default = MAX]; // The pooling method
  optional float dropout_ratio = 12 [default = 0.5]; // dropout ratio

  optional uint32 local_size = 13 [default = 5]; // for local response norm
  optional float alpha = 14 [default = 1.]; // for local response norm
  optional float beta = 15 [default = 0.75]; // for local response norm
  optional float k = 22 [default = 1.];

  // For data layers, specify the data source
  optional string source = 16;
  // For data pre-processing, we can do simple scaling and subtracting the
  // data mean, if provided. Note that the mean subtraction is always carried
  // out before scaling.
  optional float scale = 17 [default = 1];
  optional string meanfile = 18;
  // For data layers, specify the batch size.
  optional uint32 batchsize = 19;
  // For data layers, specify if we would like to randomly crop an image.
  optional uint32 cropsize = 20 [default = 0];
  // For data layers, specify if we want to randomly mirror data.
  optional bool mirror = 21 [default = false];

  // The blobs containing the numeric parameters of the layer
  repeated BlobProto blobs = 50;
  // The ratio that is multiplied on the global learning rate. If you want to
  // set the learning ratio for one blob, you need to set it for all blobs.
  repeated float blobs_lr = 51;
  // The weight decay that is multiplied on the global weight decay.
  repeated float weight_decay = 52;

  // The rand_skip variable is for the data layer to skip a few data points
  // to avoid all asynchronous sgd clients to start at the same point. The skip
  // point would be set as rand_skip * rand(0,1). Note that rand_skip should not
  // be larger than the number of keys in the database.
  optional uint32 rand_skip = 53 [default = 0];

  // Fields related to detection (det_*)
  // foreground (object) overlap threshold
  optional float det_fg_threshold = 54 [default = 0.5];
  // background (non-object) overlap threshold
  optional float det_bg_threshold = 55 [default = 0.5];
  // Fraction of batch that should be foreground objects
  optional float det_fg_fraction = 56 [default = 0.25];

  // optional bool OBSOLETE_can_clobber = 57 [default = true];

  // Amount of contextual padding to add around a window
  // (used only by the window_data_layer)
  optional uint32 det_context_pad = 58 [default = 0];

  // Mode for cropping out a detection window
  // warp: cropped window is warped to a fixed size and aspect ratio
  // square: the tightest square around the window is cropped
  optional string det_crop_mode = 59 [default = "warp"];

  // For ReshapeLayer, one needs to specify the new dimensions.
  optional int32 new_num = 60 [default = 0];
  optional int32 new_channels = 61 [default = 0];
  optional int32 new_height = 62 [default = 0];
  optional int32 new_width = 63 [default = 0];

  // Whether or not ImageLayer should shuffle the list of files at every epoch.
  // It will also resize images if new_height or new_width are not zero.
  optional bool shuffle_images = 64 [default = false];

  // For ConcatLayer, one needs to specify the dimension for concatenation, and
  // the other dimensions must be the same for all the bottom blobs.
  // By default it will concatenate blobs along the channels dimension.
  optional uint32 concat_dim = 65 [default = 1];

  optional HDF5OutputParameter hdf5_output_param = 1001;
}

// PReLU层参数，ReLU的进化版本
message PReLUParameter {
  // Parametric ReLU described in K. He et al, Delving Deep into Rectifiers:
  // Surpassing Human-Level Performance on ImageNet Classification, 2015.

  // Initial value of a_i. Default is a_i=0.25 for all i.
  // 参数初始化
  optional FillerParameter filler = 1;
  // Whether or not slope parameters are shared across channels.
  // 是否在各通道共享参数
  optional bool channel_shared = 2 [default = false];
} 
