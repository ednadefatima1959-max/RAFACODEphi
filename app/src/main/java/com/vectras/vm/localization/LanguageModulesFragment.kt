package com.vectras.vm.localization

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.ProgressBar
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.fragment.app.Fragment
import androidx.lifecycle.lifecycleScope
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.vectras.vm.R
import kotlinx.coroutines.launch

/**
 * Fragment for managing language modules.
 * Allows users to download, select, and delete language packs.
 */
class LanguageModulesFragment : Fragment() {

    private lateinit var localeManager: LocaleManager
    private lateinit var adapter: LanguageModuleAdapter
    private lateinit var recyclerView: RecyclerView
    private lateinit var descriptionText: TextView
    private lateinit var totalSizeText: TextView
    private lateinit var clearAllButton: Button
    private lateinit var loadingProgress: ProgressBar

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        return inflater.inflate(R.layout.fragment_language_modules, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        localeManager = LocaleManager.getInstance(requireContext())

        // Initialize views
        recyclerView = view.findViewById(R.id.language_modules_recycler)
        descriptionText = view.findViewById(R.id.description_text)
        totalSizeText = view.findViewById(R.id.total_size_text)
        clearAllButton = view.findViewById(R.id.clear_all_button)
        loadingProgress = view.findViewById(R.id.loading_progress)

        descriptionText.text = getString(R.string.lang_modules_description)

        setupRecyclerView()
        setupClearAllButton()
        updateTotalSize()
    }

    private fun setupRecyclerView() {
        val currentLanguage = localeManager.getCurrentLanguage()
        
        adapter = LanguageModuleAdapter(
            modules = localeManager.getAllLanguageModules(),
            onDownloadClick = { module -> downloadModule(module) },
            onDeleteClick = { module -> confirmDeleteModule(module) },
            onSelectClick = { module -> selectModule(module) },
            selectedLanguage = currentLanguage
        )

        recyclerView.layoutManager = LinearLayoutManager(requireContext())
        recyclerView.adapter = adapter
    }

    private fun setupClearAllButton() {
        clearAllButton.setOnClickListener {
            AlertDialog.Builder(requireContext())
                .setTitle(R.string.lang_clear_all)
                .setMessage(R.string.lang_clear_all_confirm)
                .setPositiveButton(R.string.ok) { _, _ ->
                    localeManager.clearAllModules()
                    adapter.updateModules(localeManager.getAllLanguageModules())
                    adapter.setSelectedLanguage("en")
                    updateTotalSize()
                    showRestartDialog()
                }
                .setNegativeButton(R.string.cancel, null)
                .show()
        }
    }

    private fun downloadModule(module: LanguageModule) {
        loadingProgress.visibility = View.VISIBLE
        
        lifecycleScope.launch {
            val success = localeManager.downloadLanguageModule(module.languageCode) { progress ->
                requireActivity().runOnUiThread {
                    adapter.updateProgress(module.languageCode, progress)
                }
            }

            loadingProgress.visibility = View.GONE
            
            if (success) {
                Toast.makeText(
                    requireContext(),
                    R.string.lang_download_success,
                    Toast.LENGTH_SHORT
                ).show()
                adapter.updateModules(localeManager.getAllLanguageModules())
                updateTotalSize()
            } else {
                Toast.makeText(
                    requireContext(),
                    R.string.lang_download_failed,
                    Toast.LENGTH_SHORT
                ).show()
                adapter.updateProgress(module.languageCode, 0)
            }
        }
    }

    private fun confirmDeleteModule(module: LanguageModule) {
        AlertDialog.Builder(requireContext())
            .setTitle(module.languageName)
            .setMessage(R.string.lang_delete_confirm)
            .setPositiveButton(R.string.remove) { _, _ ->
                localeManager.deleteLanguageModule(module.languageCode)
                adapter.updateModules(localeManager.getAllLanguageModules())
                adapter.setSelectedLanguage(localeManager.getCurrentLanguage())
                updateTotalSize()
                
                Toast.makeText(
                    requireContext(),
                    R.string.lang_delete_success,
                    Toast.LENGTH_SHORT
                ).show()
                
                if (module.languageCode == localeManager.getCurrentLanguage()) {
                    showRestartDialog()
                }
            }
            .setNegativeButton(R.string.cancel, null)
            .show()
    }

    private fun selectModule(module: LanguageModule) {
        if (!module.isDownloaded && !module.isBuiltIn) {
            Toast.makeText(
                requireContext(),
                R.string.lang_download_not_available,
                Toast.LENGTH_SHORT
            ).show()
            return
        }

        localeManager.setCurrentLanguage(module.languageCode)
        adapter.setSelectedLanguage(module.languageCode)
        
        Toast.makeText(
            requireContext(),
            getString(R.string.lang_selected, module.languageName),
            Toast.LENGTH_SHORT
        ).show()
        
        showRestartDialog()
    }

    private fun showRestartDialog() {
        AlertDialog.Builder(requireContext())
            .setTitle(R.string.lang_restart_required)
            .setMessage(R.string.lang_restart_required)
            .setPositiveButton(R.string.ok) { dialog, _ ->
                dialog.dismiss()
                // Optionally restart the app
                requireActivity().recreate()
            }
            .setNegativeButton(R.string.later, null)
            .show()
    }

    private fun updateTotalSize() {
        val sizeBytes = localeManager.getDownloadedModulesSize()
        val sizeKb = sizeBytes / 1024
        val sizeMb = sizeKb / 1024
        
        val sizeText = when {
            sizeMb > 0 -> "$sizeMb MB"
            sizeKb > 0 -> "$sizeKb KB"
            else -> "0 KB"
        }
        
        totalSizeText.text = getString(R.string.lang_total_size, sizeText)
    }

    companion object {
        fun newInstance(): LanguageModulesFragment {
            return LanguageModulesFragment()
        }
    }
}
