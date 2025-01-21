import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription 
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node
import xacro

def generate_launch_description():
    
    robotXacroname = 'rover'
    namePackage = 'gazebo'
    modelFileRelativePath = 'model/rover.xacro'
    worldFileRelativePath = 'model/world.world'
    rvizFileRelativePath = 'rviz/custom_config.rviz'
    pathModelFile = os.path.join(get_package_share_directory(namePackage),modelFileRelativePath)
    pathWorldFile = os.path.join(get_package_share_directory(namePackage),worldFileRelativePath)
    robotDescription = xacro.process_file(pathModelFile).toxml()
    gazebo_rosPackageLaunch = PythonLaunchDescriptionSource(os.path.join(get_package_share_directory('gazebo_ros'),'launch','gazebo.launch.py'))
    gazeboLaunch = IncludeLaunchDescription(gazebo_rosPackageLaunch,launch_arguments={'world': pathWorldFile}.items())
    spawnModelNode = Node(package='gazebo_ros',executable='spawn_entity.py',arguments=['-topic','robot_description', '-entity',robotXacroname],output = 'screen')
    nodeRobotStatePublisher = Node(package='robot_state_publisher',executable='robot_state_publisher',output = 'screen',parameters=[{'robot_description' : robotDescription,'use_sim_time':True}])
    
    
    rvizConfigFile = os.path.join(get_package_share_directory(namePackage), rvizFileRelativePath)
    rvizNode = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        output='screen',
        arguments=['-d', rvizConfigFile]
    )

    # Node to execute the custom C++ file
    customCppNode = Node(
        package='gazebo',  # Replace with your package name
        executable='gps',  # Replace with your C++ executable name
        name='gpstogps',
        output='screen'
    )
    
    
    launchDescriptionObject = LaunchDescription()
    launchDescriptionObject.add_action(gazeboLaunch)
    launchDescriptionObject.add_action(spawnModelNode)
    launchDescriptionObject.add_action(nodeRobotStatePublisher)
    launchDescriptionObject.add_action(rvizNode)
    launchDescriptionObject.add_action(customCppNode)

    return launchDescriptionObject
    



    