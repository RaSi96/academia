import pandas as pd

# --------------------------------------------------------------------------- #
def data_viewer(data, max_threshold = 5):
    """
    Description:
    ---------------
    Basic EDA data viewing function displaying shape, head, tail, percentiled
    descriptive statistics and per-feature data types, null values and number
    of unique values. The distrubtion of unique values is also printed as a
    percentage of the total number of columns.

    Parameters:
    ---------------
    data = the dataframe to use.
    max_threshold = the maximum number of unique values per feature to use when
                    checking how many columns have these many unique values.
                    (default = 3 unique values per feature.)

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
    dtype = data.dtypes.to_frame().transpose().set_axis(["dtype"])
    descr = data.describe(percentiles = [0.25, 0.50, 0.75, 0.90, 0.99],
                          include = "all")
    nullv = data.isnull().sum().to_frame().transpose().set_axis(["nulls"])
    nuniq = data.nunique().to_frame().transpose().set_axis(["uniques"])

    summary = pd.concat([dtype, descr, nullv, nuniq],
                        axis = 0, ignore_index = False)
    summary.rename(mapper = {"unique": "n_categories",
                             "top": "top_category",
                             "freq": "modal_categ"},
                   axis = "index",
                   inplace = True)

    with pd.option_context("display.float_format", "{:.1f}".format,
                           "display.max_columns", data.shape[1]):
        display(summary)

    print("\n--------------------\nData types:\n--------------------")
    print(data.dtypes.value_counts())

    print("\n--------------------\nNull values:\n--------------------")
    print(data.isnull().sum().value_counts())

    print("\n--------------------\nUniques:\n--------------------")
    for test in range(1, max_threshold, 1):
        ucols = (nuniq.transpose().uniques == test).sum()
        ratio = (ucols / len(data.columns)) * 100
        print("{0} columns have {1} unique value(s) [{2:.2f}%]"
              .format(ucols, test, ratio))

    print("\n====================================================\n")


# --------------------------------------------------------------------------- #
def u_count(data, u_val = 5, verbose = True):
    """
    Description:
    ---------------
    A simple function to retrieve the features in a dataframe that have less
    than u_val number of unique values. For example if a dataframe has features
    with less than 5 unique values, those features will qualify. Essentially a
    wrapper to df[col].value_counts(normalize = True).

    Parameters:
    ---------------
    data = the dataframe to use.
    u_val = the number of unique values that a feature should have.
            (default = 5)
    verbose = if True, print the distribution of every feature that qualifies.

    Returns:
    ---------------
    A list of names of all the features that qualified.
    """
    c_cols = []
    for col in data.columns[(data.nunique() < u_val)]:
        c_cols.append(col)
        if verbose is True:
            print(data[col].value_counts(normalize = True))

    return c_cols


# --------------------------------------------------------------------------- #
def u_skew(data, u_dist = 0.80, verbose = True):
    """
    Description:
    ---------------
    A simple sister-function to u_count that retrieves the names of features
    in a dataframe that have an observation with a value count percentage of
    greater than u_dist. For example, if a feature with 3 values is distributed
    as:
    a    0.8156
    b    0.0943
    c    0.0901
    the feature will qualify since its dominant value constitues 80% of the
    feature's data.

    Parameters:
    ---------------
    data = the dataframe to use.
    u_dist = the decimal percentage distribution that observations in a feature
             should have. Should be between 0 and 1.
             (default = 0.80)
    verbose = (bool) if True, print the name of every feature that qualifies.

    Returns:
    ---------------
    A list of names of all the features that qualified.
    """
    s_cols = []
    if u_dist < 0 or u_dist > 1:
        raise Exception("u_dist should be between 0 and 1.")
    else:
        for col in data.columns:
            if data[col].value_counts(normalize = True).values[0] > u_dist:
                s_cols.append(col)
                if verbose is True:
                    print(col)

        return s_cols

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
    data = the dataframe to use.
    sum_eva = the number of PCA components to sum.
              (default = 5)
    standardised (bool) = if true, the data has been standardised already.
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
    PCA.
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
        return data_standard, data_pca, pca_varatio
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
    X = the dataframe containing independent features.
    y = the dataframe containing the target feature.
    split_size = the desired size of the train/test split.
                 (default = 0.30 or 30%.)
    rand_seed = the random seed to use when splitting.
                (default = 1337.)

    Returns:
    ---------------
     X_train, X_test, y_train, y_test.
    """
    X_train, X_test, y_train, y_test = train_test_split(X, y,
                                                        test_size = split_size,
                                                        random_state = seed,
                                                        stratify = y)
    print("X_train shape:", X_train.shape)
    print("X_test shape:", X_test.shape)
    return X_train, X_test, y_train, y_test


# --------------------------------------------------------------------------- #
def model_evaluation(actuals, predictions):
    """
    Description:
    ---------------
    A function to print out formatted evaluation metrics that gauge classifier
    models' performance. Uses sklearn.metrics' confusion_matrix(),
    classification_report() and accuracy_score().

    Parameters:
    ---------------
    actuals = the target variable's dataframe. This is usually of the form:
              y_train and/or y_test; the actual data that you are trying to
              predict.
    predictions = the predicted values for y as a dataframe. This is usually of
                  the form: pred_train and/or pred_test; the predictions that
                  the model provides for estimating the values of y.

    Returns:
    ---------------
    Nothing.
    """
    print('Confusion Matrix:')
    cmatrix = pd.DataFrame(confusion_matrix(actuals, predictions),
                           columns = ["Predicted NO", "Predicted YES"],
                           index = ["Actual NO", "Actual YES"])
    display(cmatrix)

    print('\nReport: ')
    print(classification_report(actuals, predictions))

    print('\nAccuracy Score :', accuracy_score(actuals, predictions))
    print("-------------------------------------------------------------")


# --------------------------------------------------------------------------- #
