using System;
using System.Collections.Generic;
using System.Text;

namespace SceneEditor
{
    /// <summary>
    /// Event args class used by the 
    /// IMessageBus.SelectedObjectChanged event
    /// </summary>
    public class SelectedObjectChangedArgs : EventArgs
    {
        CoS.Object m_object = null;

        public SelectedObjectChangedArgs(CoS.Object obj)
        {
            m_object = obj;
        }

        /// <summary>
        /// Newly selected object
        /// </summary>
        public CoS.Object Object
        {
            get
            {
                return m_object;
            }
        }
    }

    /// <summary>
    /// Event args class used by the 
    /// IMessageBus.Scene* events
    /// </summary>
    public class SceneEventArgs : EventArgs
    {
        CoS.Game.Scene m_scene = null;

        public SceneEventArgs(CoS.Game.Scene scene)
        {
            m_scene = scene;
        }

        public CoS.Game.Scene Scene
        {
            get
            {
                return m_scene;
            }
        }
    }

    /// <summary>
    /// Event args class used by the 
    /// IMessageBus.Timer event
    /// </summary>
    public class TimerEventArgs : EventArgs
    {
        double m_time = 0;

        public TimerEventArgs(double time)
        {
            m_time = time;
        }

        public double DeltaT
        {
            get
            {
                return m_time;
            }
        }
    }

    /// <summary>
    /// This interface is implemented by a central system, through 
    /// which all framework messages should go. Interested parties
    /// can subscribe to various messages, and anyone with a handle 
    /// to an implementation of this interface can place events on 
    /// the bus (raise events, in other words).
    /// </summary>
    public interface IMessageBus
    {
        /*
         * Object Selection Change Notification
         *  
         * In an editor, with N possible perspectives on the data
         * which you are editing, it's entirely possible to have 
         * multiple views open on the same data set, and if you 
         * change the datum of focus in one view, you want all other
         * views on that data to change as well. Handling/raising
         * events in this section performs that notification.
         */

        /// <summary>
        /// Subscribe to this event to be notified when the currently 
        /// selected object has changed somewhere in the system.
        /// </summary>
        event EventHandler<SelectedObjectChangedArgs> SelectedObjectChanged;

        /// <summary>
        /// Provided for external sources to raise this event on an instance
        /// of this interface
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="obj"></param>
        void RaiseSelectedObjectChanged(object sender, CoS.Object obj);


        /*
         * Scene Loading/Saving Event
         * 
         * When a scene is loaded, unloaded or saved, notification should
         * be sent to all interested parties.
         */

        /// <summary>
        /// Called when a new scene has been loaded into the editor
        /// </summary>
        event EventHandler<SceneEventArgs> SceneLoaded;
        /// <summary>
        /// Called when a scene has been unloaded from the editor
        /// </summary>
        event EventHandler<SceneEventArgs> SceneUnloaded;
        /// <summary>
        /// Called when the current scene has been saved to disk.
        /// </summary>
        event EventHandler<SceneEventArgs> SceneSaved;
        
        /// <summary>
        /// Call to notify of new scene loading
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="scene"></param>
        void RaiseSceneLoaded(object sender, CoS.Game.Scene scene);
        /// <summary>
        /// Call to notify of scene unloading
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="scene"></param>
        void RaiseSceneUnloaded(object sender, CoS.Game.Scene scene);
        /// <summary>
        /// Call to notify of scene saving (to persistent storage of some kind)
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="scene"></param>
        void RaiseSceneSaved(object sender, CoS.Game.Scene scene);

        /*
         * We want a central place to get our timer tick, but we don't want
         * it to come in off another thread, so we let anyone post their own
         * internal timer ticks (for example, windows that have an infinite 
         * Invalidate() loop going to keep getting control back in their Paint
         * even handlers), and only send out ticks at controlled intervals.
         */

        /// <summary>
        /// Raised when a prescribed time interval has passed (i.e. 10ms)
        /// </summary>
        event EventHandler<TimerEventArgs> Timer;

        /// <summary>
        /// Can be raised by anyone who wants to advance the central
        /// timer; events will only be broadcast when a particular interval
        /// has passed, so this method can be called as often as anyone
        /// likes.
        /// </summary>
        void RaiseTimerTick(object sender);
    }
}
