#include <config_utilities/factory.h>
#include <hydra/common/semantic_color_map.h>
#include <hydra_llm/embedding_distances.h>
#include <hydra_llm/embedding_group.h>
#include <hydra_ros/visualizer/dsg_visualizer_plugin.h>
#include <llm/ClipVector.h>
#include <ros/ros.h>
#include <std_srvs/SetBool.h>
#include <visualization_msgs/MarkerArray.h>

namespace hydra::llm {

using NodeColor = SemanticNodeAttributes::ColorVector;

class LLMPlacesVisualizer : public DsgVisualizerPlugin {
 public:
  LLMPlacesVisualizer(const ros::NodeHandle& nh, const std::string& name);

  virtual ~LLMPlacesVisualizer();

  void draw(const ConfigManager& configs,
            const std_msgs::Header& header,
            const DynamicSceneGraph& graph) override;

  void reset(const std_msgs::Header& header, const DynamicSceneGraph& graph) override;

  bool hasChange() const override;

  void clearChangeFlag() override;

  NodeColor getNodeColor(const ConfigManager& configs,
                         const SceneGraphNode& node) const;

  void addMarkerToArray(const visualization_msgs::Marker& marker,
                        visualization_msgs::MarkerArray& msg);

  void handleNewTask(const ::llm::ClipVector& msg);

  bool handleService(std_srvs::SetBool::Request& req, std_srvs::SetBool::Response& res);

  void resetTasks();

  std::string ns_;
  ros::Publisher pub_;
  ros::Subscriber sub_;
  ros::ServiceServer srv_;

  bool need_redraw_;
  bool color_by_task_;
  EmbeddingGroup::Ptr tasks_;
  std::unique_ptr<EmbeddingDistance> metric_;
  bool has_current_task_feature_;
  Eigen::VectorXd current_task_feature_;
  std::set<std::string> published_markers_;
  SemanticColorMap::Ptr colormap_;

  inline static const auto registration_ =
      config::Registration<DsgVisualizerPlugin,
                           LLMPlacesVisualizer,
                           ros::NodeHandle,
                           std::string>("LLMPlacesVisualizer");
};

}  // namespace hydra::llm
