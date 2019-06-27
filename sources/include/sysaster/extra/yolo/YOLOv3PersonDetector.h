#ifndef _YOLO_PERSON_DETECTOR_
#define _YOLO_PERSON_DETECTOR_

#include "sysaster/core/detector/PersonDetector.h"
#include "sysaster/core/detector/DetectionResultData.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/dnn.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "extern/base64.h"

#define INP_WIDTH 416
#define INP_HEIGHT 416
#define CONF_THRESHOLD 0.1
#define NMS_THRESHOLD 0.3

/**
 * YOLO detector class.
 *
 * @author Vitor Greati
 * */
class YOLOv3PersonDetector : public PersonDetector {

    private:

        std::string file_class_names;
        std::string file_config;
        std::string file_weight;

        std::vector<std::string> classes;

        mutable cv::dnn::Net net;

        bool errored {false};
        
        bool load_classes() {
            std::ifstream if_file_class {file_class_names.c_str()};
            if (if_file_class.good()) {
                std::string line;
                while (std::getline(if_file_class, line))
                    classes.push_back(line);    
                return true;
            }
            return false;
        }

        std::vector<cv::String> get_output_names() const {
            static std::vector<cv::String> names;
            if(names.empty()){
                // Get indices of output
                std::vector<int> outputLayers = net.getUnconnectedOutLayers();
                // Get name of internal layers
                std::vector<cv::String> layersNames = net.getLayerNames();
                // get name of output layers
                names.resize(outputLayers.size());
                for(auto i = 0; i < outputLayers.size(); ++i)
                    names[i] = layersNames[outputLayers[i] - 1];
            }
            return names;
        }

        void save_pred(int classId, float conf, const cv::Rect& _box, 
                const std::vector<std::string> & classes,
                const cv::Mat& frame,
                std::vector<DetectionResultData>& detect_data_results) const {
        
	    std::cout << "estou executando0" << std::endl;
            DetectionResultData drd;

            int x = 0, y = 0, w = 0, h = 0;
            x = std::max(0, _box.x);
            y = std::max(0, _box.y);
            w = std::min(_box.width + _box.x, frame.cols - _box.x);
            h = std::min(_box.height + _box.y, frame.rows - _box.y);

            cv::Rect box (x, y, w, h);

            drd.x = box.x;
            drd.y = box.y;
            drd.width = box.width;
            drd.height = box.height;
            drd.confidence = conf;

	    std::cout << "estou executando1" << std::endl;
        std::cout << box << std::endl;
        std::cout << frame.rows << frame.cols << frame.channels() << std::endl;
	    std::cout << "estou executando1_" << std::endl;
            if (!frame.data || frame.empty()) return;

            //> Crop and separate channels
            cv::Mat clip_original = frame(box);

            //cv::resize(clip_original, clip_original, cv::Size(50, 50));

	    /*std::chrono::time_point<std::chrono::system_clock> now = 
		        std::chrono::system_clock::now();
	    auto duration = now.time_since_epoch();
	    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	    cv::imwrite(std::to_string(millis)+".jpg", clip_original);*/
	    std::cout << "estou executando2" << std::endl;

            drd.channels = clip_original.channels();
            std::vector<cv::Mat> chans;
            cv::split(clip_original, chans);
            //> Stack channels
            cv::Mat clip;
            for (auto& c : chans)
                clip.push_back(c);
            //> Encode b64
            std::string clipped = base64_encode(clip.data, drd.width * drd.height * drd.channels);
            drd.clipped_image = new char[clipped.length() + 1];
            strcpy(drd.clipped_image, clipped.c_str());
            drd.clipped_image_size = clipped.length() + 1;

            detect_data_results.push_back(drd);
        }
            
        void post_process(const cv::Mat & frame, const std::vector<cv::Mat> & outs,
                std::vector<DetectionResultData>& detect_data_results) const {
            std::vector<int> classIds;
            std::vector<float> confidences;
            std::vector<cv::Rect> boxes;

            for(size_t i = 0; i < outs.size(); ++i){
                float * data = (float*) outs[i].data;
                for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols){
                    cv::Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
                    cv::Point classIdPoint;
                    double confidence;

                    // Get the value and location of max confidence
                    cv::minMaxLoc(scores, 0, &confidence, &classIdPoint);

                    if(confidence > CONF_THRESHOLD){
                        int cx = (int)(data[0] * frame.cols);
                        int cy = (int)(data[1] * frame.rows);
                        int w  = (int)(data[2] * frame.cols);
                        int h  = (int)(data[3] * frame.rows);
                        int l = cx - w / 2;
                        int t = cy - h / 2;

                        classIds.push_back(classIdPoint.x);
                        confidences.push_back((float) confidence);
                        boxes.push_back(cv::Rect(l,t,w,h));
                    }
                }
            }

            // Eliminate redudant boxes based in confidence and position
            std::vector<int> indices;
            cv::dnn::NMSBoxes(boxes, confidences, CONF_THRESHOLD, NMS_THRESHOLD, indices);
            for(size_t i = 0; i < indices.size(); ++i){
                int idx = indices[i];
                cv::Rect box = boxes[idx];
		std::cout << "class " << classes[classIds[idx]] << std::endl;
                save_pred(classIds[idx], confidences[idx], box, classes, frame, detect_data_results);
            }
        }

    public:

        YOLOv3PersonDetector() {}

        /**
         * Construct a detector, loading the network 
         * from given weights.
         * */
        YOLOv3PersonDetector(const decltype(file_class_names)& file_class_names,
                const decltype(file_config)& file_config,
                const decltype(file_weight)& file_weight)
                : file_class_names{file_class_names}, file_config {file_config}, file_weight {file_weight} {
       
            errored = not load_classes();

            if (!errored) {
                try {
                    net = cv::dnn::readNetFromDarknet(file_config, file_weight);
                    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
                    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
                } catch (std::exception& e) {
                    errored = true;
                    std::cout << e.what() << std::endl;
                }
            }
        }

        /**
         * Performs person detection given an image.
         *
         * @param image     image to be analysed
         * @param results   detection results 
         * */
        void detect(const cv::Mat image, std::vector<DetectionResultData>& detect_data_results) const override {

            std::cout << "asked to detect" << std::endl;

            cv::Point anchor {-1, -1};

            double delta = 0.0;
            int ddepth = -1;

            if(image.size().width <= 600 || image.size().height <= 600){
                int kernel_sz = 3;
                cv::Mat kernel = (-1) * cv::Mat::ones(kernel_sz, kernel_sz, CV_32F);
                kernel.at<float>(1,1)= 5.0;
                kernel.at<float>(0,0)= 0.0;
                kernel.at<float>(0,2)= 0.0;
                kernel.at<float>(2,0)= 0.0;
                kernel.at<float>(2,2)= 0.0;
                cv::filter2D(image, image, ddepth , kernel, anchor, delta, cv::BORDER_DEFAULT );
            }

            cv::Mat blob;

            cv::dnn::blobFromImage(image, blob, 1/255.0, cv::Size(INP_WIDTH, INP_HEIGHT), cv::Scalar(0,0,0), true, false);

            //> Set network input
            net.setInput(blob);

            //> Run inference
            std::vector<cv::Mat> rect_results;
            net.forward(rect_results, this->get_output_names());

            //> Post process
            post_process(image, rect_results, detect_data_results);

            //> Add time and id
            long ms = std::chrono::duration_cast< std::chrono::milliseconds >(
                std::chrono::system_clock::now().time_since_epoch()
            ).count();
            for (auto& r : detect_data_results) {
                r.id_frame = ms;    // TODO compute better id_frame
                r.time_ms = ms;     // TODO include time when captured
            }

            //DetectionResultData r;
            /*r.height = image.rows; 
            r.width = image.cols; 

            std::vector<uchar> buf;
            cv::imencode(".jpg", image, buf);
            memcpy(r.clipped_image, buf.data(), buf.size());
            results.push_back(r);*/
        }

        /**
         * Indicates whether the loading presented error.
         *
         * @return errored model
         * */
        inline bool is_errored() const override { return errored; }

};

#endif

