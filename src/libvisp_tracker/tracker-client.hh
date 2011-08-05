#ifndef VISP_TRACKER_TRACKER_CLIENT_HH
# define VISP_TRACKER_TRACKER_CLIENT_HH
# include <boost/filesystem/path.hpp>

# include <dynamic_reconfigure/server.h>

# include <image_proc/advertisement_checker.h>

# include <image_transport/image_transport.h>
# include <image_transport/subscriber_filter.h>

# include <message_filters/subscriber.h>
# include <message_filters/sync_policies/approximate_time.h>
# include <message_filters/synchronizer.h>

# include <sensor_msgs/Image.h>
# include <sensor_msgs/CameraInfo.h>

# include <visp_tracker/MovingEdgeConfig.h>
# include <visp_tracker/MovingEdgeSites.h>
# include <visp_tracker/TrackingResult.h>
# include <visp_tracker/TrackingMetaData.h>

# include <visp/vpCameraParameters.h>
# include <visp/vpHomogeneousMatrix.h>
# include <visp/vpImage.h>
# include <visp/vpMbEdgeTracker.h>
# include <visp/vpPose.h>


namespace visp_tracker
{
  class TrackerClient
  {
  public:
    typedef vpImage<unsigned char> image_t;
    typedef std::vector<vpPoint> points_t;
    typedef std::vector<vpImagePoint> imagePoints_t;

    typedef dynamic_reconfigure::Server<visp_tracker::MovingEdgeConfig>
    reconfigureSrv_t;

    typedef dynamic_reconfigure::Server
    <visp_tracker::MovingEdgeConfig>::CallbackType
    reconfigureCallback_t;


    TrackerClient(unsigned queueSize = 5u);

    void spin();
  protected:
    /// \brief Make sure the topics we subscribe already exist.
    void checkInputs();

    void loadModel();

    vpHomogeneousMatrix loadInitialPose();
    void saveInitialPose(const vpHomogeneousMatrix& cMo);
    points_t loadInitializationPoints();

    void initClick();
    void initPoint(unsigned& i,
		   points_t& points,
		   imagePoints_t& imagePoints,
		   ros::Rate& rate,
		   vpPose& pose);


    void waitForImage();

    void sendcMo(const vpHomogeneousMatrix& cMo);
  private:
    unsigned queueSize_;

    ros::NodeHandle nodeHandle_;
    image_transport::ImageTransport imageTransport_;

    image_t image_;

    std::string modelPath_;
    std::string modelName_;

    std::string cameraPrefix_;
    std::string trackerPrefix_;
    std::string rectifiedImageTopic_;
    std::string cameraInfoTopic_;

    std::string initService_;

    boost::filesystem::path vrmlPath_;
    boost::filesystem::path initPath_;

    image_transport::CameraSubscriber cameraSubscriber_;

    reconfigureSrv_t reconfigureSrv_;

    std_msgs::Header header_;
    sensor_msgs::CameraInfoConstPtr info_;

    vpMe movingEdge_;
    vpCameraParameters cameraParameters_;
    vpMbEdgeTracker tracker_;

    /// \brief Helper used to check that subscribed topics exist.
    image_proc::AdvertisementChecker checkInputs_;
  };
} // end of namespace visp_tracker.

#endif //! VISP_TRACKER_TRACKER_CLIENT_HH