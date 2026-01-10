package com.vectras.vm.localization

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageButton
import android.widget.ImageView
import android.widget.ProgressBar
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.vectras.vm.R

/**
 * Adapter for displaying language modules in a RecyclerView.
 * Shows download status, progress, and allows downloading/deleting modules.
 */
class LanguageModuleAdapter(
    private var modules: List<LanguageModule>,
    private val onDownloadClick: (LanguageModule) -> Unit,
    private val onDeleteClick: (LanguageModule) -> Unit,
    private val onSelectClick: (LanguageModule) -> Unit,
    private var selectedLanguage: String = "en"
) : RecyclerView.Adapter<LanguageModuleAdapter.ViewHolder>() {

    class ViewHolder(view: View) : RecyclerView.ViewHolder(view) {
        val languageName: TextView = view.findViewById(R.id.language_name)
        val nativeName: TextView = view.findViewById(R.id.native_name)
        val statusIcon: ImageView = view.findViewById(R.id.status_icon)
        val downloadProgress: ProgressBar = view.findViewById(R.id.download_progress)
        val actionButton: ImageButton = view.findViewById(R.id.action_button)
        val selectedIndicator: View = view.findViewById(R.id.selected_indicator)
        val moduleSize: TextView = view.findViewById(R.id.module_size)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        val view = LayoutInflater.from(parent.context)
            .inflate(R.layout.item_language_module, parent, false)
        return ViewHolder(view)
    }

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        val module = modules[position]
        
        holder.languageName.text = module.languageName
        holder.nativeName.text = module.nativeName
        
        // Show module size for non-built-in modules
        if (module.isBuiltIn) {
            holder.moduleSize.text = holder.itemView.context.getString(R.string.lang_built_in)
        } else {
            val sizeKb = module.sizeBytes / 1024
            holder.moduleSize.text = holder.itemView.context.getString(R.string.lang_module_size, sizeKb)
        }

        // Handle download progress
        if (module.downloadProgress in 1..99) {
            holder.downloadProgress.visibility = View.VISIBLE
            holder.downloadProgress.progress = module.downloadProgress
            holder.statusIcon.visibility = View.GONE
            holder.actionButton.visibility = View.GONE
        } else {
            holder.downloadProgress.visibility = View.GONE
            holder.statusIcon.visibility = View.VISIBLE
            holder.actionButton.visibility = View.VISIBLE
        }

        // Set status icon and action button based on state
        if (module.isDownloaded || module.isBuiltIn) {
            holder.statusIcon.setImageResource(R.drawable.ic_check_circle)
            holder.statusIcon.setColorFilter(
                holder.itemView.context.getColor(android.R.color.holo_green_dark)
            )
            
            if (module.isBuiltIn) {
                holder.actionButton.visibility = View.GONE
            } else {
                holder.actionButton.setImageResource(R.drawable.ic_delete)
                holder.actionButton.setOnClickListener { onDeleteClick(module) }
            }
        } else {
            holder.statusIcon.setImageResource(R.drawable.ic_cloud_download)
            holder.statusIcon.setColorFilter(
                holder.itemView.context.getColor(android.R.color.darker_gray)
            )
            holder.actionButton.setImageResource(R.drawable.ic_download)
            holder.actionButton.setOnClickListener { onDownloadClick(module) }
        }

        // Show selected indicator
        val isSelected = module.languageCode == selectedLanguage
        holder.selectedIndicator.visibility = if (isSelected) View.VISIBLE else View.GONE

        // Allow selection only for downloaded modules
        holder.itemView.setOnClickListener {
            if (module.isDownloaded || module.isBuiltIn) {
                onSelectClick(module)
            }
        }
        
        // Visual feedback for selectable items
        holder.itemView.alpha = if (module.isDownloaded || module.isBuiltIn) 1.0f else 0.6f
    }

    override fun getItemCount() = modules.size

    /**
     * Update the list of modules
     */
    fun updateModules(newModules: List<LanguageModule>) {
        modules = newModules
        notifyDataSetChanged()
    }

    /**
     * Update download progress for a specific module
     */
    fun updateProgress(languageCode: String, progress: Int) {
        val index = modules.indexOfFirst { it.languageCode == languageCode }
        if (index >= 0) {
            modules = modules.toMutableList().apply {
                this[index] = this[index].copy(downloadProgress = progress)
            }
            notifyItemChanged(index)
        }
    }

    /**
     * Update the selected language
     */
    fun setSelectedLanguage(languageCode: String) {
        val oldIndex = modules.indexOfFirst { it.languageCode == selectedLanguage }
        val newIndex = modules.indexOfFirst { it.languageCode == languageCode }
        selectedLanguage = languageCode
        if (oldIndex >= 0) notifyItemChanged(oldIndex)
        if (newIndex >= 0) notifyItemChanged(newIndex)
    }
}
