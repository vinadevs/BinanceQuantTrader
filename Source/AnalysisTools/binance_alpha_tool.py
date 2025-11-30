#*******************************************************************************
# COPYRIGHT NOTES
# ---------------
# This is a part of Binance Quant Trader Project
# Copyright(C) - vinadevs
# This source code can be used, distributed or modified under Apache license
#*******************************************************************************

# binance_alpha_tool.py

# Simple tool for loading local Binance market data, engineering features, and detecting alpha.

# import pandas as pd import numpy as np from sklearn.preprocessing import
# StandardScaler from sklearn.linear_model import LogisticRegression from
# sklearn.model_selection import TimeSeriesSplit from sklearn.pipeline
# import Pipeline

class LocalMarketLoader: def init(self, time_col_candidates=None):
self.m_time_cols = time_col_candidates or ['open_time','timestamp','time','date']

    def load(self, path: str, time_unit_ms: bool = False) -> pd.DataFrame:
        if path.lower().endswith('.parquet'):
            df = pd.read_parquet(path)
        else:
            df = pd.read_csv(path)

        cols_lower = {c.lower(): c for c in df.columns}
        colmap = {}
        for candidate in ['open','high','low','close','volume']:
            for k,v in cols_lower.items():
                if k == candidate or k.startswith(candidate):
                    colmap[v] = candidate
                    break

        time_col = None
        for cand in self.m_time_cols:
            if cand in cols_lower:
                time_col = cols_lower[cand]
                break

        if time_col is None and (df.index.dtype == 'datetime64[ns]' or np.issubdtype(df.index.dtype, np.datetime64)):
            df.index.name = 'open_time'
        else:
            if time_col is None:
                for c in df.columns:
                    if np.issubdtype(df[c].dtype, np.integer) and df[c].median() > 1e9:
                        time_col = c
                        break
            if time_col is None:
                raise ValueError("Time column not found.")

            if np.issubdtype(df[time_col].dtype, np.integer) and time_unit_ms:
                df['open_time'] = pd.to_datetime(df[time_col], unit='ms')
            else:
                df['open_time'] = pd.to_datetime(df[time_col])
            df = df.set_index('open_time')

        df = df.rename(columns=colmap)
        for req in ['open','high','low','close','volume']:
            if req not in df.columns:
                raise ValueError(f"Required column {req} missing.")
        for c in ['open','high','low','close','volume']:
            df[c] = pd.to_numeric(df[c], errors='coerce')

        return df.sort_index()

class FeatureEngineer: def add_features(self, df: pd.DataFrame) ->
pd.DataFrame: df = df.copy() df['return'] = df['close'].pct_change()
df['log_return'] = np.log(df['close']).diff()

        df['ema_12'] = df['close'].ewm(span=12, adjust=False).mean()
        df['ema_26'] = df['close'].ewm(span=26, adjust=False).mean()
        df['mom_5'] = df['close'] / df['close'].shift(5) - 1
        df['vol_20'] = df['log_return'].rolling(20).std()

        delta = df['close'].diff()
        up = delta.clip(lower=0)
        down = -1 * delta.clip(upper=0)
        roll_up = up.rolling(14).mean()
        roll_down = down.rolling(14).mean()
        rs = roll_up / roll_down
        df['rsi_14'] = 100 - (100 / (1 + rs))

        df['target'] = (df['close'].shift(-1) > df['close']).astype(int)
        return df.dropna()

class AlphaModel: def init(self): self.m_model = Pipeline([ ('scaler',
StandardScaler()), ('clf', LogisticRegression(max_iter=200)) ])

    def train(self, df: pd.DataFrame, features):
        X = df[features]
        y = df['target']
        self.m_model.fit(X, y)

    def predict(self, df: pd.DataFrame, features):
        return self.m_model.predict(df[features])

if name == "main": print("This module provides LocalMarketLoader, FeatureEngineer, and AlphaModel.")
