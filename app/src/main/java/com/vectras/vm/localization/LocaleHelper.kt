package com.vectras.vm.localization

import android.app.Activity
import android.content.Context
import android.content.ContextWrapper
import android.os.Build
import android.os.LocaleList
import java.util.Locale

/**
 * Helper class for applying locale changes to activities.
 * Activities should call LocaleHelper.applyLocale() in attachBaseContext().
 */
object LocaleHelper {

    /**
     * Applies the saved locale to the context.
     * Call this from Activity.attachBaseContext()
     */
    fun applyLocale(context: Context): Context {
        val localeManager = LocaleManager.getInstance(context)
        return localeManager.applyLocale(context)
    }

    /**
     * Wraps the context with the selected locale configuration.
     * Use this for creating context-aware resources.
     */
    fun wrap(context: Context): ContextWrapper {
        val localeManager = LocaleManager.getInstance(context)
        val languageCode = localeManager.getCurrentLanguage()
        val locale = Locale(languageCode)

        Locale.setDefault(locale)

        val configuration = context.resources.configuration

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
            configuration.setLocales(LocaleList(locale))
        } else {
            @Suppress("DEPRECATION")
            configuration.locale = locale
        }

        val newContext = context.createConfigurationContext(configuration)
        return ContextWrapper(newContext)
    }

    /**
     * Recreates the activity to apply locale changes.
     */
    fun recreateActivity(activity: Activity) {
        activity.recreate()
    }

    /**
     * Gets the current locale code.
     */
    fun getCurrentLocale(context: Context): String {
        return LocaleManager.getInstance(context).getCurrentLanguage()
    }

    /**
     * Sets the current locale and optionally recreates the activity.
     */
    fun setLocale(context: Context, languageCode: String, recreateActivity: Activity? = null) {
        LocaleManager.getInstance(context).setCurrentLanguage(languageCode)
        recreateActivity?.recreate()
    }
}
