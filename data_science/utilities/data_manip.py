import numpy as np
import pandas as pd

from sklearn.decomposition import PCA
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split
from sklearn.metrics import confusion_matrix, classification_report


# --------------------------------------------------------------------------- #
def data_viewer(data, max_threshold = 5):
    """
    Description:
    ---------------
    Basic EDA data viewing function displaying shape, head, tail, percentiled
    descriptive statistics and per-feature data types, null values, uniques and
    IQR. The distrubtion of unique values is also printed as a percentage of the
    total number of columns.

    Parameters:
    ---------------
    data [pd.DataFrame] = the dataframe to use.
    max_threshold [int] = the maximum number of unique values per feature to
                          use when scanning.
                          (default = 3)

    Returns:
    ---------------
    Nothing.
    """
    print("\n--------------------\nShape:\n--------------------")
    print(data.shape)

    print("\n--------------------\nHead:\n--------------------")
    with pd.option_context("display.max_columns", data.shape[1]):
        display(data.head(10))

    print("\n--------------------\nTail:\n--------------------")
    with pd.option_context("display.max_columns", data.shape[1]):
        display(data.tail(10))

    print("\n--------------------\nSummary:\n--------------------")
    # first we extract our numerical columns and calculate the IQR per feature
    num_cols = data.select_dtypes(exclude = "object").columns
    iqrs = []

    for col in data.columns:
        if col not in num_cols:
            iqrs.append(np.nan)
        else:
            itp = "midpoint"
            q1 = 0.25
            q3 = 0.75

            col_iqr = data[col].quantile(q3, itp) - data[col].quantile(q1, itp)
            iqrs.append(col_iqr)

    # then we begin carefully crafting our summary statistics dataframe
    desc = data.describe(percentiles = [0.25, 0.50, 0.75, 0.90, 0.99],
                         include = "all").transpose()

    desc["unique"] = data.nunique().to_frame("unique")
    desc.insert(0, "dtype", data.dtypes.values)
    desc.insert(1, "nulls", data.isnull().sum().values)
    desc.insert(7, "IQR", iqrs)
    desc.rename(mapper = {"top": "modal_categ", "freq": "modal_freq"},
                axis = "columns", inplace = True)

    # and finally we display our carefully crafted summary statistics
    with pd.option_context("display.float_format", "{:.1f}".format,
                           "display.max_columns", desc.shape[0]):
        display(desc.transpose())

    print("\n--------------------\nIntrospection:\n--------------------")
    print(data.info())

    print("\n--------------------\nUniques:\n--------------------")
    for test in range(1, max_threshold, 1):
        ucols = (data.nunique().values == test).sum()
        ratio = (ucols / len(data.columns)) * 100
        print("{0} columns have {1} unique value(s) [{2:.2f}%]"
              .format(ucols, test, ratio))

    print("\n====================================================\n")


# --------------------------------------------------------------------------- #
def u_count(data, u_val = 5, exact = False):
    """
    Description:
    ---------------
    A simple function to retrieve the features in a dataframe that have either
    exactly or up to u_val number of unique values. "u" implies "unique".

    Parameters:
    ---------------
    data [pd.DataFrame] = the dataframe to use.
    u_val [int] = the number of unique values that a feature should have.
                  (default = 5)
    exact[bool] = if True, only features with exactly u_val number of unique
                  values will qualify.
                  (default = False)

    Returns:
    ---------------
    A list of names of all the features that qualified.
    """
    c_cols = []

    if exact is False:
        for col in data.columns[data.nunique() <= u_val]:
            c_cols.append(col)
    else:
        for col in data.columns[data.nunique() == u_val]:
            c_cols.append(col)

    return c_cols


# --------------------------------------------------------------------------- #
def u_skew(data, u_dist = 0.90):
    """
    Description:
    ---------------
    A function that retrieves the names of features in a dataframe that have an
    observation account for greater than or exactly u_dist percent. For example
    if a feature with 3 values is distributed as:
    a    0.9156
    b    0.0534
    c    0.0310
    the feature will qualify since its dominant value constitues 90% and above
    of the feature's data. "u" implies "unique".

    Parameters:
    ---------------
    data [pd.DataFrame] = the dataframe to use.
    u_dist [int] = the percentage distribution that observations in a feature
                   should have. Should be between 0 and 1.
                   (default = 0.90)

    Returns:
    ---------------
    A list of names of all the features that qualified.
    """
    s_cols = []

    if u_dist < 0 or u_dist > 1:
        raise Exception("u_dist should be between 0 and 1.")
    else:
        for col in data.columns:
            if data[col].value_counts(normalize = True).values[0] >= u_dist:
                s_cols.append(col)

        return s_cols


# --------------------------------------------------------------------------- #
def get_contrib(data, against, minority = True):
    """
    A function that breaks down the contributors to a feature's minority or
    majority value. Useful in understanding the level of skew within a feature.

    If a dataframe feature has a skewed distribution (one value >=85%), instead
    of removing the column outright it helps to understand whether the dominant
    or minority value can be used to regress against or classify multiple other
    values in another feature (usually the target vector.) If so, removing such
    a skewed column would result in a direct loss of workable information.

    Parameters:
    ---------------
    data [pd.DataFrame / pd.Series] = the main dataframe to use.
    against [pd.Series] = the column within which to check contributors.
    minority [bool] = if False, use the feature's dominant value instead.
                      (default = True)

    Returns:
    ---------------
    Nothing.

    Notes:
    ---------------
    1. The output text of Series.value_counts() is always terminated with the
    name of the series passed as "against." This kind of behaviour is inherent
    to pd.Series.value_counts().
    """
    if isinstance(data, pd.core.series.Series):
        # if we were sent in a single feature (pd.Series) we should concatenate
        # it into a dataframe along with the target feature and use the result.
        if minority is False:
            dom_val = data.value_counts().index[0]
        else:
            dom_val = data.value_counts().index[-1]

        col = data.name

        data = pd.concat([data, against], axis = 1)
        print("Column \"{}\" value \"{}\" has these contributors:"
              .format(col, dom_val))
        print("--------------------")
        print((data.loc[data[col] == dom_val])[against.name]
              .value_counts(normalize = True))

    else:
        # otherwise if we were sent in a dataframe (pd.DataFrame) we should
        # loop through all the columns and print out the target feature's value
        # counts corresponding to the dataframe column's dominant value.
        if against.name not in data.columns:
            data = pd.concat([data, against], axis = 1)

        for col in data.columns:
            if minority is False:
                dom_val = data[col].value_counts().index[0]
            else:
                dom_val = data[col].value_counts().index[-1]

            if col == against.name:
                continue
            print("Column \"{}\" value \"{}\" has these contributors:"
                  .format(col, dom_val))
            print("--------------------")
            print((data.loc[data[col] == dom_val])[against.name]
                  .value_counts(normalize = True), "\n")
            print("\n")


# --------------------------------------------------------------------------- #
def standard_pca(data, sum_eva = 5, standardised = True):
    """
    Description:
    ---------------
    A function to apply a PCA analysis to a given dataset. The sum of the first
    sum_eva components (default = 5) is also printed. The default assumption is
    that the data has already been standardised.

    Parameters:
    ---------------
    data [pd.DataFrame] = the dataframe to use.
    sum_eva [int] = the number of PCA components to sum.
                    (default = 5)
    standardised [bool] = if true, the data has been standardised already.
                          (default = True)

    Returns:
    ---------------
    data_pca = the PCA dataframe.
    pca_varatio = the pca_variance_ratio list.
    (if standardised = False)
    data_standard = the standardised dataframe.

    Notes:
    ---------------
    If standardised = False (i.e. the data hasn't been standardised), then
    StandardScaler() is used to standardise the data before running the PCA.
    The instance of StandardScaler() that was fitted to and transformed the
    data is not returned.
    """
    pca = PCA()

    if standardised == False:
        data_standard = StandardScaler().fit_transform(data)
        data_pca = pca.fit_transform(data_standard)
    else:
        data_pca = pca.fit_transform(data)

    pca_varatio = sorted(pca.explained_variance_ratio_, reverse = True)

    print("\n--------------------\nVariance Ratio:\n--------------------\n")
    print(pca_varatio)

    print("\n--------------------\nVariance Ratio sum:\n--------------------\n")
    print(sum(pca_varatio[0:sum_eva]))

    if standardised == False:
        return data_pca, pca_varatio, data_standard
    else:
        return data_pca, pca_varatio


# --------------------------------------------------------------------------- #
def prepare_data(X, y, split_size = 0.30, seed = 1337):
    """
    Description:
    ---------------
    A handy wrapper function around train_test_split with automatic stratifying
    against the target variable 'y'.

    Parameters:
    ---------------
    X [pd.dataFrame] = the dataframe containing independent features.
    y [pd.Series] = the target feature.
    split_size [int] = the desired size of the train/test split.
                       (default = 0.30)
    rand_seed [int] = the random seed to use when splitting.
                      (default = 1337)

    Returns:
    ---------------
    X_train, X_test, y_train, y_test.
    """
    X_train, X_test, y_train, y_test = train_test_split(X, y, stratify = y,
                                                        test_size = split_size,
                                                        random_state = seed)
    print("Training:\n--------------------")
    print("X_train shape:", X_train.shape)
    print("y_train dist:", y_train.value_counts(normalize = True))
    print("--------------------\n\n")

    print("Testing:\n--------------------")
    print("X_test shape:", X_test.shape)
    print("y_test dist:", y_train.value_counts(normalize = True))
    print("--------------------")
    return X_train, X_test, y_train, y_test


# --------------------------------------------------------------------------- #
def model_evaluation(actual, preds):
    """
    Description:
    ---------------
    A function to print out formatted evaluation metrics that gauge classifier
    models' performance. Uses sklearn.metrics' confusion_matrix() and
    classification_report(). This function only handles binary classification.
    The labels of the confusion matrix are internally provided as [1, 0] so as
    to keep the orientation reliable (x = [yes, no], y = [yes, no]).

    Parameters:
    ---------------
    actuals [pd.Series / np.array] = the target variable's values. This is of
                                     the form y_train and/or y_test; the actual
                                     data that you are trying to predict.
    predictions [pd.Series / np.array] = the predicted values for y. This is of
                                         the form pred_train and/or pred_test;
                                         the model's predictions for estimating
                                         the values of y_train/y_test.
    Returns:
    ---------------
    Nothing.
    """
    labs = [1, 0]

    print('Confusion Matrix:')
    cmatrix = pd.DataFrame(confusion_matrix(actual, preds, labels = labs),
                           columns = ["Predicted YES", "Predicted NO"],
                           index = ["Actual YES", "Actual NO"])
    display(cmatrix)

    print('\nReport: ')
    print(classification_report(actual, preds))
    print("-------------------------------------------------------------")


# --------------------------------------------------------------------------- #
